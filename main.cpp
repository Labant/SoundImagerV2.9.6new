#include <QtWidgets/QApplication>
#include <QSplashScreen>
#include <QDir>
#include "MainViewControl.h"
#include "MainModel.h"
//#include "MainViewController.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	//��������
	QPixmap pix(QDir::currentPath()+"/image/main_ls800x600.png");
	QSplashScreen splash(pix);
	splash.show();
	a.processEvents();

	//view/viewController and some VM dealwith
	MainViewControl w;

	//some subClass model dealwith
	MainModel model;
	model.setView(&w);
	w.setModel(&model);

	//�ڶ��ַ����������С����Ҫ�̳�
	//MainViewController w;

	splash.finish(&w);

    w.show();
    return a.exec();
}
