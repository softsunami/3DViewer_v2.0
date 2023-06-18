#include "tranformation.h"

namespace s21 {

void MoveStrategy::Transform(std::vector<double> &vertex_coord, double &step,
                             transformation_t &axis) {
  for (std::size_t i = axis; i < vertex_coord.size(); i += 3) {
    vertex_coord[i] += step;
  }
}

void RotateStrategy::Transform(std::vector<double> &vertex_coord, double &angle,
                               transformation_t &axis) {
  double tempAngle = angle * M_PI / 180;
  double cosValue = cos(tempAngle);
  double sinValue = sin(tempAngle);

  for (std::size_t i = (axis == kX) ? 1 : 0; i < vertex_coord.size(); i += 3) {
    double coord = vertex_coord[i];

    if (axis == kX) {
      vertex_coord[i] = cosValue * coord - sinValue * vertex_coord[i + 1];
      vertex_coord[i + 1] = sinValue * coord + cosValue * vertex_coord[i + 1];
    } else if (axis == kY) {
      vertex_coord[i] = cosValue * coord + sinValue * vertex_coord[i + 2];
      vertex_coord[i + 2] = -sinValue * coord + cosValue * vertex_coord[i + 2];
    } else if (axis == kZ) {
      vertex_coord[i] = cosValue * coord + sinValue * vertex_coord[i + 1];
      vertex_coord[i + 1] = -sinValue * coord + cosValue * vertex_coord[i + 1];
    }
  }
}

void ScaleStrategy::Transform(std::vector<double> &vertex_coord, double &scale,
                              transformation_t &) {
  for (double &i : vertex_coord) {
    i *= scale;
  }
}

Strategy::Strategy() : strategy_(nullptr) {}

void Strategy::SetStrategy(TransformationStrategy *strategy) {
  strategy_ = strategy;
}

void Strategy::PerformTransformation(std::vector<double> &vertex_coord,
                                     double &value, transformation_t &axis) {
  strategy_->Transform(vertex_coord, value, axis);
  delete strategy_;
}

}  // namespace s21
