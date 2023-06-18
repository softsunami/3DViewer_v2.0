#ifndef CPP4_3DVIEWER_V2_0_MODEL_TRANSFORMATION_H_
#define CPP4_3DVIEWER_V2_0_MODEL_TRANSFORMATION_H_

#include <cmath>
#include <vector>

namespace s21 {

enum transformation_t {
  kX = 0,
  kY = 1,
  kZ = 2,
  kMove = 0,
  kRotate = 1,
  kScale = 2
};

class TransformationStrategy {
 public:
  virtual ~TransformationStrategy() = default;
  virtual void Transform(std::vector<double>&, double&, transformation_t&) {}
};

class MoveStrategy : public TransformationStrategy {
  void Transform(std::vector<double>& vertex_coord, double& step,
                 transformation_t& axis) override;
};

class RotateStrategy : public TransformationStrategy {
  void Transform(std::vector<double>& vertex_coord, double& angle,
                 transformation_t& axis) override;
};

class ScaleStrategy : public TransformationStrategy {
  void Transform(std::vector<double>& vertex_coord, double& scale,
                 transformation_t&) override;
};

class Strategy {
 public:
  Strategy();
  void SetStrategy(TransformationStrategy* strategy);
  void PerformTransformation(std::vector<double>& vertex_coord, double& value,
                             transformation_t& axis);

 private:
  TransformationStrategy* strategy_;
};

}  // namespace s21

#endif  // CPP4_3DVIEWER_V2_0_MODEL_TRANSFORMATION_H_
