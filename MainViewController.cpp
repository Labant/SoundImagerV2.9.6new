#include "MainViewController.h"
#include "MainViewControl.h"
#include "MainModel.h"

MainViewController::MainViewController(QWidget* parent)
{

	//this->setWindowTitle(QString::fromLocal8Bit("声源定位 | 苏州理声科技"));
	//this->setWindowIcon(QIcon(QPixmap(QDir::currentPath() + "/image/logo.png")));

	////view/viewController and some VM dealwith
	//this->_pVC = new MainViewControl(this);

	////some subClass model dealwith
	//this->_pMM = new MainModel(this);
	//this->_pMM->setView(this->_pVC);
	//this->_pVC->setModel(this->_pMM);
}

MainViewController::~MainViewController()
{
}
