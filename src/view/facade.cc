#include "view.h"

namespace s21 {

void Facade::MoveModel(QString &slider_name) {
  double rate = 0;
  if (slider_name == "horizontalSlider_move_x") {
    rate = ui_->horizontalSlider_move_x->value() / 30.0;
    emit SetTransform(0, rate - transform_info_.move_x, 0);
    transform_info_.move_x = rate;
  } else if (slider_name == "horizontalSlider_move_y") {
    rate = ui_->horizontalSlider_move_y->value() / 30.0;
    emit SetTransform(0, rate - transform_info_.move_y, 1);
    transform_info_.move_y = rate;
  } else if (slider_name == "horizontalSlider_move_z") {
    rate = ui_->horizontalSlider_move_z->value() / 30.0;
    emit SetTransform(0, rate - transform_info_.move_z, 2);
    transform_info_.move_z = rate;
  }
}

void Facade::RotateModel(QString &slider_name) {
  if (slider_name == "horizontalSlider_rotate_x") {
    emit SetTransform(
        1, ui_->horizontalSlider_rotate_x->value() - transform_info_.rotate_x,
        0);
    transform_info_.rotate_x = ui_->horizontalSlider_rotate_x->value();
  } else if (slider_name == "horizontalSlider_rotate_y") {
    emit SetTransform(
        1, ui_->horizontalSlider_rotate_y->value() - transform_info_.rotate_y,
        1);
    transform_info_.rotate_y = ui_->horizontalSlider_rotate_y->value();
  } else if (slider_name == "horizontalSlider_rotate_z") {
    emit SetTransform(
        1, ui_->horizontalSlider_rotate_z->value() - transform_info_.rotate_z,
        2);
    transform_info_.rotate_z = ui_->horizontalSlider_rotate_z->value();
  }
}

void Facade::ScaleModel(QString &slider_name) {
  if (slider_name == "horizontalSlider_scale") {
    double rate = pow(10, ui_->horizontalSlider_scale->value() / 60.0) / 100;
    if (transform_info_.scale == 0) transform_info_.scale = rate;
    emit SetTransform(2, rate / transform_info_.scale, 0);
    transform_info_.scale = rate;
  }
}

void Facade::Transform(QSlider *slider) {
  QString slider_name = slider->objectName();

  MoveModel(slider_name);
  RotateModel(slider_name);
  ScaleModel(slider_name);
}

}  // namespace s21
