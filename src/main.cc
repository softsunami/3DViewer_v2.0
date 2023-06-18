#include <QApplication>

#include "controller/controller.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  s21::View view;
  view.setWindowTitle("3D Viewer");
  s21::Controller controller(&view);

  QObject::connect(
      &controller,
      SIGNAL(SolutionReady(std::vector<int> *, std::vector<double> *)), &view,
      SLOT(HandleSolution_(std::vector<int> *, std::vector<double> *)));
  QObject::connect(&controller, SIGNAL(ErrorHasOccured()), &view,
                   SLOT(HandleError_()));

  view.show();
  return QApplication::exec();
}
