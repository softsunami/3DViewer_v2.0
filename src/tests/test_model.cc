#include "test_core.h"

TEST(Model, CorrectFile) {
  s21::Model *model;
  model = &s21::Model::GetInstance();

  model->SetFileName("tests/models/cub.obj");
  model->Parser();

  EXPECT_EQ(model->GetError(), 0);
  EXPECT_EQ(model->GetVertexCoord().size(), 24);
  EXPECT_EQ(model->GetVertexIndex().size(), 48);
}

TEST(Model, WrongExtension) {
  s21::Model *model;
  model = &s21::Model::GetInstance();

  model->SetFileName("Makefile");
  model->Parser();

  EXPECT_EQ(model->GetError(), s21::kFileWrongExtension);
  EXPECT_TRUE(model->GetVertexCoord().empty());
  EXPECT_TRUE(model->GetVertexIndex().empty());
}

TEST(Model, FailedToOpen) {
  s21::Model *model;
  model = &s21::Model::GetInstance();

  model->SetFileName("qwerty.obj");
  model->Parser();

  EXPECT_EQ(model->GetError(), s21::kFailedToOpen);
  EXPECT_TRUE(model->GetVertexCoord().empty());
  EXPECT_TRUE(model->GetVertexIndex().empty());
}

TEST(Model, IncorrectFile) {
  s21::Model *model;
  model = &s21::Model::GetInstance();

  model->SetFileName("tests/models/incorrect.obj");
  model->Parser();

  EXPECT_EQ(model->GetError(), s21::kIncorrectData);
}

TEST(Model, Move) {
  s21::Model *model;
  model = &s21::Model::GetInstance();

  model->SetFileName("tests/models/cub.obj");
  model->Parser();

  std::vector<double> vertex_copy(model->GetVertexCoord());
  auto axis_x = s21::kX, axis_y = s21::kY, axis_z = s21::kZ;
  int strategy = s21::kMove;
  double value = 1;
  model->Transform(strategy, value, axis_x);
  model->Transform(strategy, value, axis_y);
  model->Transform(strategy, value, axis_z);

  for (std::size_t i = 0; i < vertex_copy.size(); i++) {
    EXPECT_DOUBLE_EQ(model->GetVertexCoord()[i], vertex_copy[i] + 1);
  }
}

TEST(Model, Rotate) {
  s21::Model *model;
  model = &s21::Model::GetInstance();

  model->SetFileName("tests/models/cub.obj");
  model->Parser();

  std::vector<double> vertex_copy(model->GetVertexCoord());
  auto axis_x = s21::kX, axis_y = s21::kY, axis_z = s21::kZ;
  int strategy = s21::kRotate;
  double value = 5;
  model->Transform(strategy, value, axis_x);
  model->Transform(strategy, value, axis_y);
  model->Transform(strategy, value, axis_z);

  for (std::size_t i = 0; i < vertex_copy.size(); i++) {
    if (vertex_copy[i] != 0) {
      EXPECT_FALSE(model->GetVertexCoord()[i] == vertex_copy[i]);
    }
  }
}

TEST(Model, Scale) {
  s21::Model *model;
  model = &s21::Model::GetInstance();

  model->SetFileName("tests/models/cub.obj");
  model->Parser();

  std::vector<double> vertex_copy(model->GetVertexCoord());
  auto axis_x = s21::kX;
  int strategy = s21::kScale;
  double value = 5;
  model->Transform(strategy, value, axis_x);

  for (std::size_t i = 0; i < vertex_copy.size(); i++) {
    EXPECT_DOUBLE_EQ(model->GetVertexCoord()[i], vertex_copy[i] * 5);
  }
}
