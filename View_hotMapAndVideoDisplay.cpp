#include "View_hotMapAndVideoDisplay.h"

View_hotMapAndVideoDisplay::View_hotMapAndVideoDisplay(QWidget* parent)
{
	this->init();
	//this->setBackground(QPixmap::fromImage(QImage("C:/Users/lvs/Pictures/png/9439BD31B85F7377FC79CF2A6BE1FFBB.jpg")), true, Qt::KeepAspectRatio);
}

View_hotMapAndVideoDisplay::~View_hotMapAndVideoDisplay()
{
}

void View_hotMapAndVideoDisplay::initMember()
{
	// set up the QCPColorMap:
	//this->colorMap = new QCPColorMap(this->xAxis, this->yAxis);
	this->colorMap = new QCPColorMap(this->mCustomPlot.xAxis, this->mCustomPlot.yAxis);//不显，用于存储裁剪范围来计算最大值
	this->colorMap->setAntialiased(true);
	//IMX291
	//manual
	this->colorMap->data()->setSize(30 * 2, 30 * 2); // 我们希望彩色地图有nx*ny的数据点
	this->colorMap->data()->setRange(QCPRange(0, 360), QCPRange(0, 180)); // 并在键（x）和值（y）维上跨越坐标范围

	////// 添加色标:
////QCPColorScale* colorScale = new QCPColorScale(this);
////this->plotLayout()->addElement(0, 1, colorScale); // 将其添加到主轴矩形的右侧
////colorScale->setType(QCPAxis::atRight); // 刻度应为垂直条，刻度线/坐标轴标签右侧（实际上，右侧已经是默认值）
////colorMap->setColorScale(colorScale); // 将颜色图与色标关联
////colorScale->axis()->setLabel("Magnetic Field Strength");


	//添加系数改变大小
	static QCPColorGradient cpColorGradient;
	cpColorGradient.clearColorStops();
	//cpColorGradient.setPeriodic()//周期性
	//cpColorGradient.setLevelCount(360);//默认值350
	cpColorGradient.setColorStopAt(0, QColor(Qt::transparent));//透明
	cpColorGradient.setColorStopAt(posColor[1] + this->mHmSizeFactor * 2.0f - 0.001, QColor(Qt::transparent));//透明
	//空间比：0.02，0.03，0.01，0.01 共6份

	//cpColorGradient.setColorStopAt(posColor[0] + this->mHmSizeFactor * 2.0f, hotMapCoefficient > 0 ? QColor(115, 115, 255, 150) : QColor(Qt::transparent));//紫罗兰色 不显示
	cpColorGradient.setColorStopAt(posColor[1] + this->mHmSizeFactor * 3.0f, hotMapCoefficient > 0 ? QColor(77, 255, 255, 150) : QColor(Qt::transparent));//亮蓝色
	cpColorGradient.setColorStopAt(posColor[2] + this->mHmSizeFactor * 1.0f, hotMapCoefficient > 0 ? QColor(255, 255, 0, 150) : QColor(Qt::transparent));//明黄色
	cpColorGradient.setColorStopAt(posColor[3] + this->mHmSizeFactor * 1.0f, hotMapCoefficient > 0 ? QColor(255, 71, 26, 150) : QColor(Qt::transparent));//正红色
	cpColorGradient.setColorStopAt(posColor[4], hotMapCoefficient > 0 ? QColor(255, 71, 26, 255) : QColor(Qt::transparent));//正红色 255
	//cpColorGradient.setColorStopAt(posColor[4], hotMapCoefficient > 0 ? QColor(102, 20, 0, 150) : QColor(Qt::transparent));//红黑色 不好看

	//cpColorGradient.setColorStops()
	cpColorGradient.setColorInterpolation(QCPColorGradient::ColorInterpolation::ciHSV);//ciHSV
	colorMap->setGradient(cpColorGradient);

	//test
	//colorMap->setGradient(QCPColorGradient::gpJet);

	// 重新缩放数据维度（颜色），以使所有数据点都位于颜色渐变显示的范围内:
	this->colorMap->rescaleDataRange(true);

	// 重新缩放键（x）和值（y）轴，以便可以看到整个颜色图：
	this->rescaleAxes();
}

void View_hotMapAndVideoDisplay::initLayout()
{
	//热力图网格不可见
	this->xAxis->grid()->setVisible(false);
	this->yAxis->grid()->setVisible(false);

	//热力图x y 轴不可见
	this->xAxis->setVisible(false);
	this->xAxis2->setVisible(false);
	this->yAxis->setVisible(false);
	this->yAxis2->setVisible(false);

	// configure axis rect:配置轴矩形
	//this->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom); // 这也将允许通过拖拽/缩放尺度改变颜色范围
	this->axisRect()->setupFullAxesBox(true);
	this->xAxis->setLabel("x");
	this->yAxis->setLabel("y");
	this->xAxis->setVisible(false);
	this->yAxis->setVisible(false);
	this->xAxis2->setVisible(false);
	this->yAxis2->setVisible(false);
}

void View_hotMapAndVideoDisplay::init()
{
	this->initLayout();
	this->initMember();
}
