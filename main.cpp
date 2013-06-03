#include "mainwindow.hpp"
#include <QApplication>
#include "scene3D.h"
#include <qwt_plot.h>
#include <qwt_plot_curve.h>

int main(int argc, char *argv[])
{

	QApplication a(argc, argv);
    Scene3D scene1;

	MainWindow mw;
	mw.show();

	mw.scene = &scene1;

    scene1.setWindowTitle("CopterModel");
    scene1.resize(500, 500);
    scene1.show();


	return a.exec();
}
