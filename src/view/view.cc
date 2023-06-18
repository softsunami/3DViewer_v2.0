#include "view.h"

#include <QMessageBox>

namespace s21 {

View::View(QWidget *parent)
    : QOpenGLWidget(parent), ui_(new Ui::View), facade(new Facade(ui_)) {
  setlocale(LC_NUMERIC, "C");
  ui_->setupUi(this);
  this->resize(1440, 1080);
  ui_->tabWidget_save_format->tabBar()->hide();
  setAcceptDrops(true);
  InitDefaultSettings_();
  ConnectSlotSignals_();
  last_settings_ = new QSettings("SAVE_3DVIEWER", "3DViewer", this);
  RestoreSettings_();
}

View::~View() {
  SaveSettings_();
  delete last_settings_;
  delete facade;
  delete ui_;
}

void View::ConnectSlotSignals_() {
  connect(ui_->pushButton_open_file, SIGNAL(clicked()), this,
          SLOT(OpenFileBtnClicked_()));

  connect(ui_->buttonGroupTabs, SIGNAL(buttonClicked(QAbstractButton *)), this,
          SLOT(ChangeTab_(QAbstractButton *)));
  connect(ui_->saveAsButton, SIGNAL(clicked()), this,
          SLOT(ChangeSaveFormatTab_()));

  connect(ui_->buttonGroupImg, SIGNAL(buttonClicked(QAbstractButton *)), this,
          SLOT(Record_(QAbstractButton *)));

  std::vector<QSlider *> transform_sliders = {
      ui_->horizontalSlider_move_x,   ui_->horizontalSlider_move_y,
      ui_->horizontalSlider_move_z,   ui_->horizontalSlider_rotate_x,
      ui_->horizontalSlider_rotate_y, ui_->horizontalSlider_rotate_z,
      ui_->horizontalSlider_scale};

  for (auto slider : transform_sliders) {
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(TransformModel_()));
  }

  connect(ui_->horizontalSlider_vert_size, SIGNAL(valueChanged(int)), this,
          SLOT(update()));
  connect(ui_->horizontalSlider_edges_thick, SIGNAL(valueChanged(int)), this,
          SLOT(update()));
  connect(ui_->comboBox, SIGNAL(currentIndexChanged(int)), this,
          SLOT(update()));
  connect(ui_->comboBox_disp_method, SIGNAL(currentIndexChanged(int)), this,
          SLOT(update()));
  connect(ui_->comboBox_disp_method_2, SIGNAL(currentIndexChanged(int)), this,
          SLOT(update()));

  connect(ui_->buttonGroupColors, SIGNAL(buttonClicked(QAbstractButton *)),
          this, SLOT(SetColor_(QAbstractButton *)));
}

void View::InitDefaultSettings_() {
  QColor bgColor = ui_->pushButton_bg_color->palette().button().color();
  QColor eColor = ui_->pushButton_edges_color->palette().button().color();
  QColor vColor = ui_->pushButton_vertex_color->palette().button().color();
  settings_.bg_color.setNamedColor(bgColor.name());
  settings_.edge_color.setNamedColor(eColor.name());
  settings_.vertex_color.setNamedColor(vColor.name());
  ui_->tabWidget->setCurrentIndex(0);
}

void View::SetColor_(QAbstractButton *button) {
  QColor color = QColorDialog::getColor();

  if (color.isValid()) {
    QString prev_style(
        "border-radius: 3px;\nborder: 1px solid rgb(110, 105, 157);\n");
    if (button == ui_->pushButton_bg_color) {
      settings_.bg_color = color;
      ui_->pushButton_bg_color->setStyleSheet(
          QString(prev_style + "background-color: %1")
              .arg(settings_.bg_color.name()));
    } else if (button == ui_->pushButton_edges_color) {
      settings_.edge_color = color;
      ui_->pushButton_edges_color->setStyleSheet(
          QString(prev_style + "background-color: %1")
              .arg(settings_.edge_color.name()));
    } else if (button == ui_->pushButton_vertex_color) {
      settings_.vertex_color = color;
      ui_->pushButton_vertex_color->setStyleSheet(
          QString(prev_style + "background-color: %1")
              .arg(settings_.vertex_color.name()));
    }

    update();
  }
}

void View::ChangeTab_(QAbstractButton *button) {
  int tabIndex = ui_->tabWidget->currentIndex();

  if (button == ui_->_pushButton_transformation2 && tabIndex != 0) {
    ui_->tabWidget->setCurrentIndex(0);
  } else if (button == ui_->pushButton_settings1 && tabIndex != 1) {
    ui_->tabWidget->setCurrentIndex(1);
  }
}

void View::ChangeSaveFormatTab_() {
  ui_->tabWidget_save_format->setCurrentIndex(1);
}

void View::ClearSliders_() {
  QSlider *sliders[] = {
      ui_->horizontalSlider_move_x,   ui_->horizontalSlider_move_y,
      ui_->horizontalSlider_move_z,   ui_->horizontalSlider_rotate_x,
      ui_->horizontalSlider_rotate_y, ui_->horizontalSlider_rotate_z,
  };

  for (auto slider : sliders) {
    slider->setValue(0);
  }

  ui_->horizontalSlider_scale->setValue(100);
}

void View::OpenFileBtnClicked_() {
  QString filepath = QFileDialog::getOpenFileName(
      this, ("Select Model"), "../models/", "3D Image Files (*.obj)");

  if (filepath != "") {
    ClearSliders_();
    emit SetModel(filepath);
    ui_->label_filename->setText((QFileInfo(filepath)).fileName());
    ui_->label_filename->setStyleSheet(
        QString("background-color: %1").arg(QColor(58, 81, 113).name()));
  }
}

void View::HandleSolution_(std::vector<int> *vertex_index,
                           std::vector<double> *vertex_coord) {
  vertex_index_ = vertex_index->data();
  vertex_coord_ = vertex_coord->data();
  count_vertex_index_ = static_cast<int>(vertex_index->size());
  count_vertex_coord_ = static_cast<int>(vertex_coord->size());
  QString info = "Count of vertex: %1\nCount of edges: %2";
  ui_->label_file_info->setText(
      info.arg(count_vertex_coord_ / 3).arg(count_vertex_index_ / 4));
  update();
}

void View::HandleError_() {
  vertex_index_ = nullptr;
  vertex_coord_ = nullptr;
  count_vertex_index_ = 0;
  count_vertex_coord_ = 0;
  update();
  QMessageBox::critical(nullptr, "Error", "Плохая моделька, брат");
}

void View::TransformModel_() {
  auto *slider = qobject_cast<QSlider *>(sender());
  facade->Transform(slider);
  update();
}

void View::Record_(QAbstractButton *button) {
  if (button == ui_->pushButton_bmp)
    SaveBmp_();
  else if (button == ui_->pushButton_jpeg)
    SaveJpeg_();
  else if (button == ui_->pushButton_gif)
    SaveGif_();
}

}  // namespace s21
