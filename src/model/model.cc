#include "model.h"

namespace s21 {

void Model::Parser() {
  if (error_code_ != kFileWrongExtension) {
    std::ifstream file(filename_);
    std::string line;

    if (!file.is_open()) {
      error_code_ = kFailedToOpen;
      return;
    }

    while (error_code_ == 0 && std::getline(file, line)) {
      if (!line.compare(0, 2, "v ")) {
        VertexParser_(line);
      } else if (!line.compare(0, 2, "f ")) {
        EdgesParser_(std::string(line.begin() + 2, line.end()));
      }
    }
    Normalize_();
    file.close();
  }
}

void Model::VertexParser_(const std::string& line) {
  double d1 = 0, d2 = 0, d3 = 0;
  char j = 0;

  if (sscanf(line.c_str(), "%c %lf %lf %lf %c", &j, &d1, &d2, &d3, &j) == 4) {
    vertex_coord_.insert(vertex_coord_.end(), {d1, d2, d3});
  } else {
    error_code_ = kIncorrectData;
  }
}

void Model::EdgesParser_(const std::string& line) {
  std::istringstream iss(line);
  std::string token;
  int first = 0, first_index = 0, index = 0;

  while (std::getline(iss, token, ' ')) {
    sscanf(token.c_str(), "%d", &index);

    if (index <= 0 || token.find('\r') != std::string::npos) continue;
    --index;

    if (first == 0) {
      vertex_index_.push_back(index);
      first_index = index;
      first = 1;
    } else {
      vertex_index_.insert(vertex_index_.end(), {index, index});
    }
  }
  vertex_index_.push_back(first_index);
}

void Model::SetFileName(const std::string& file_name) {
  if (file_name.size() <= 3 ||
      file_name.compare(file_name.size() - 4, 4, ".obj")) {
    error_code_ = kFileWrongExtension;
  } else {
    filename_ = file_name;
    error_code_ = 0;
  }
  vertex_coord_.clear();
  vertex_index_.clear();
}

int Model::GetError() { return error_code_; }

std::vector<int>& Model::GetVertexIndex() { return vertex_index_; }

std::vector<double>& Model::GetVertexCoord() { return vertex_coord_; }

void Model::Transform(int& strategy_type, double& value,
                      transformation_t& axis) {
  if (strategy_type == kMove) {
    TransformationModel_.SetStrategy(new MoveStrategy());
  } else if (strategy_type == kRotate) {
    TransformationModel_.SetStrategy(new RotateStrategy());
  } else if (strategy_type == kScale) {
    TransformationModel_.SetStrategy(new ScaleStrategy());
  }
  TransformationModel_.PerformTransformation(vertex_coord_, value, axis);
}

void Model::Normalize_() {
  double max_value = 0;
  for (double value : vertex_coord_) {
    if (value > max_value) max_value = value;
  }
  if (max_value > 10) {
    for (double& value : vertex_coord_) {
      value = value / max_value;
    }
  }
}

typename Model::Model& Model::GetInstance() {
  static Model instance;
  return instance;
}

}  // namespace s21
