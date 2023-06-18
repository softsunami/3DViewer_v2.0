#include "controller.h"

namespace s21 {

void Controller::SetFileName_(const QString& file_name) {
  model_->SetFileName(file_name.toStdString());
  model_->Parser();

  if (model_->GetError() == 0) {
    emit SolutionReady(&GetVertexIndex(), &GetVertexCoord());
  } else {
    emit ErrorHasOccured();
  }
}

std::vector<int>& Controller::GetVertexIndex() {
  return model_->GetVertexIndex();
}

std::vector<double>& Controller::GetVertexCoord() {
  return model_->GetVertexCoord();
}

void Controller::Transform_(int strategy_type, double value, int axis) {
  auto temp_axis = static_cast<transformation_t>(axis);
  model_->Transform(strategy_type, value, temp_axis);
}

}  // namespace s21
