#ifndef CPP4_3DVIEWER_V2_0_CONTROLLER_CONTROLLER_H_
#define CPP4_3DVIEWER_V2_0_CONTROLLER_CONTROLLER_H_

#include "../model/model.h"
#include "../view/view.h"

namespace s21 {

class Controller : public QObject {
  Q_OBJECT

 public:
  explicit Controller(View* view) : model_(&Model::GetInstance()), view_(view) {
    QObject::connect(view_, SIGNAL(SetModel(QString)), this,
                     SLOT(SetFileName_(QString)));
    QObject::connect(view_->facade, SIGNAL(SetTransform(int, double, int)),
                     this, SLOT(Transform_(int, double, int)));
  }

  std::vector<int>& GetVertexIndex();
  std::vector<double>& GetVertexCoord();

 signals:
  void SolutionReady(std::vector<int>* vertex_index,
                     std::vector<double>* vertex_coord);
  void ErrorHasOccured();

 private slots:
  void SetFileName_(const QString& file_name);
  void Transform_(int strategy_type, double value, int axis = kX);

 private:
  Model* model_;
  View* view_;
};

}  // namespace s21

#endif  // CPP4_3DVIEWER_V2_0_CONTROLLER_CONTROLLER_H_
