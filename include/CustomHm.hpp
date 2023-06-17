/**
* function:drawCloudMap --可用，待完善
**Author: Lvs                                   **
**Date: 13.06.23                                **
**Version: 1.0.0                                **
 */

#ifndef MYPOINT_STRUCT
#define MYPOINT_STRUCT
struct MyPoint
{
	int posX;
	int posY;
	int radius;
	int count;
};//Point 结构体包含了鼠标点击widget的具体位置，和绘制圆的半径以及被点击的次数，这个次数是整体权重计算的比例
#endif //! MYPOINT_STRUCT

#ifndef CUSTOMHM_HPP
#define CUSTOMHM_HPP

#ifndef QPOINTER_H
#include <QPainter>
#define QPOINTER_H
#endif
#include <QObject>
#include <QDebug>
#include <QMutex>
#include <QRgb>

#ifndef CONFIG
#define CONFIG 1
#else 
#include "config.h"
#endif // ! CONFIG	1




class CustomHm :QObject
{

private:
	float mAspectRatio;//宽比
	float mHighRadio;//高比
	//alpha图
	QImage* _pAlphaImg = nullptr;
	//hotMap
	QImage* _pHotImg = nullptr;

protected:
	QSize mImageSize;
	//整体的透明度[0-255]
	static constexpr int HeatAlpha = 200;
	//颜色表，透明度为0
	QRgb _colorList[256];
	//转热力图
	QRgb* line_heat = nullptr;

public:
	//共享静态锁
	QMutex mMutex;
public:
	explicit CustomHm(QObject * parent = nullptr) 
	{
		this->SetColorList();
	};
	~CustomHm() {};

/**
*	\general function
*/
	void SetAspectRation(const float& aspect)
	{
		this->mAspectRatio = aspect;
	};
	void SetHightRation(const float& hight)
	{
		this->mHighRadio = hight;
	};
	void SetRation(const QSizeF& ah_sizef)
	{
		this->mAspectRatio = ah_sizef.width();
		this->mHighRadio = ah_sizef.height();
	}
	//设置云图取色列表
	void SetColorList()
	{
		//根据线性渐变色条得到颜色表
		QLinearGradient linear = QLinearGradient(QPoint(0, 0), QPoint(255, 0));//构建色条
		linear.setColorAt(0, Qt::blue);
		linear.setColorAt(0.4, Qt::blue);//蓝色
		linear.setColorAt(0.5, Qt::cyan);//天蓝色
		linear.setColorAt(0.6, Qt::green);//绿色
		linear.setColorAt(0.8, Qt::yellow);//黄色
		linear.setColorAt(0.95, Qt::red);//红色

		//把渐变色绘制到Img方便取颜色
		QImage img(256, 1, QImage::Format_ARGB32);//构建一个QImage,用于绘制渐变色
		//img = QImage(256, 1, QImage::Format_ARGB32);//构建一个QImage,用于绘制渐变色
		QPainter painter(&img);
		painter.fillRect(img.rect(), linear);//把线性色条绘制到这个QImage中

		//HeatAlpha为热力图整体透明度
		quint32 alpha = 0;

		for (quint32 i = 0; i < 256; i++) {
			//根据热力图透明度来计算颜色表的透明度
			alpha = HeatAlpha / 255.0 * i;	//HeatAlpha : 200;aplha:[0,200]

			//typedef unsigned int QRgb: format #AARRGGBB
			//颜色+透明度 _colorList：颜色表
			_colorList[i] = img.pixel(i, 0) & 0x00FFFFFF | (alpha << 24);
		}
	}
/**
 * function:-->reset to transparent for aphlamap and hotmap
 */
	void ResetAphalHotmap()
	{
		//qDebug() << __FUNCDNAME__;

		this->_pAlphaImg->fill(Qt::transparent);
		delete this->_pHotImg;
		this->_pHotImg = nullptr;
		this->_pHotImg = new QImage(this->mImageSize, QImage::Format_ARGB32);

	}
/**
 * \core function
 */
	void DrawCloudMap(QImage& src, const MyPoint position)
	{
		QMutexLocker mlocker(&this->mMutex);
		this->mImageSize = src.size();

		if (this->_pAlphaImg == nullptr)
		{
			this->_pAlphaImg = new QImage(src.size(), QImage::Format_Alpha8);
			this->_pAlphaImg->fill(Qt::transparent);//黑的的但是alpha是透明的
		}
		if (this->_pHotImg == nullptr)
		{
			this->_pHotImg = new QImage(src.size(), QImage::Format_ARGB32);
			this->_pHotImg->fill(Qt::transparent);//黑的的但是alpha是透明的
		}

		//重置云图与透明图填充
		this->ResetAphalHotmap();

		//画透明图
		{
			QPainter painter(this->_pAlphaImg);
			painter.setPen(Qt::transparent);

			QRadialGradient gradient(position.posX, position.posY, position.radius);
			gradient.setColorAt(0, QColor(0, 0, 0, 255));//255->alpha,at(0)是最中心的位置
			gradient.setColorAt(1, QColor(0, 0, 0, 100));//at(1)是以radius为半径画圆的范围内，最边缘的位置
			painter.setBrush(gradient);
			painter.drawEllipse(QPointF(position.posX, position.posY), position.radius, position.radius);
		}
		//透明度转云图
		{
			//把alpha值转为颜色值
			for (int row = 0; row < this->_pAlphaImg->height(); row++)
			{
				//dataimg QImage::Format_Alpha8，一个点1个字节
				const uchar* aplha_data = this->_pAlphaImg->scanLine(row);//获取透明图上的扫描线，共700刚行

				//heatimg QImage::Format_ARGB32，一个点4个字节
				this->line_heat = reinterpret_cast<QRgb*>(this->_pHotImg->scanLine(row));//获取热力图上的扫面线，共700行。


				for (int col = 0; col < this->_pAlphaImg->width(); col++)//把每行的扫面线取列位数，共1000列
				{
					//根据alpha透明度从颜色表取颜色
					line_heat[col];
					aplha_data[col];
					this->_colorList[aplha_data[col]];
					this->line_heat[col] = this->_colorList[aplha_data[col]];//取出的颜色再设置到_heatImg对应的地址
				}
			}
		}

		//在源图上画热力图
		{
			QPainter p(&src);
			//绘制热力图
			p.drawImage(0, 0, *this->_pHotImg);
			//qDebug() << this->_pHotImg->save("_pHotImg.jpg","jpg", 100);
		}

	};

/**
* \core this overload function 
*/
	void DrawCloudMap(QImage& src, const MyPoint position,QMutex& mutex)
	{
		//qDebug() << "11->!";//1！-2！：1ms
		//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822
		QMutexLocker mlocker(&this->mMutex);//内部锁
		//qDebug() << "12->!";//1！-2！：1ms
		//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822
		QMutexLocker mOutlocker(&mutex);//外部锁
		//qDebug() << "13->!";//1！-2！：1ms
		//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822

		this->mImageSize = src.size();

		//qDebug() << "14->!";//1！-2！：1ms
		//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822
		if (this->_pAlphaImg == nullptr)
		{
			this->_pAlphaImg = new QImage(src.size(), QImage::Format_Alpha8);
			this->_pAlphaImg->fill(Qt::transparent);//黑的的但是alpha是透明的
		}
		if (this->_pHotImg == nullptr)
		{
			this->_pHotImg = new QImage(src.size(), QImage::Format_ARGB32);
			this->_pHotImg->fill(Qt::transparent);//黑的的但是alpha是透明的
		}
		//qDebug() << "15->!";//1！-2！：1ms
		//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822

		//重置云图与透明图填充
		this->ResetAphalHotmap();//3ms

		//qDebug() << "16->!";//1！-2！：1ms
		//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822
		//画透明图
		{
			//QPainter painter(this->_pAlphaImg);
			//painter.setPen(Qt::transparent);

			//QRadialGradient gradient(position.posX, position.posY, position.radius);
			//gradient.setColorAt(0, QColor(0, 0, 0, 255));//255->alpha,at(0)是最中心的位置
			//gradient.setColorAt(1, QColor(0, 0, 0, 100));//at(1)是以radius为半径画圆的范围内，最边缘的位置
			//painter.setBrush(gradient);
			//painter.drawEllipse(QPointF(position.posX, position.posY), position.radius, position.radius);
		}
		//qDebug() << "17->!";//1！-2！：1ms
		//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822
		//透明度转云图
		{
			////把alpha值转为颜色值
			//for (int row = 0; row < this->_pAlphaImg->height(); row++)
			//{
			//	//dataimg QImage::Format_Alpha8，一个点1个字节
			//	const uchar* aplha_data = this->_pAlphaImg->scanLine(row);//获取透明图上的扫描线，共700刚行

			//	//heatimg QImage::Format_ARGB32，一个点4个字节
			//	this->line_heat = reinterpret_cast<QRgb*>(this->_pHotImg->scanLine(row));//获取热力图上的扫面线，共700行。


			//	for (int col = 0; col < this->_pAlphaImg->width(); col++)//把每行的扫面线取列位数，共1000列
			//	{
			//		//根据alpha透明度从颜色表取颜色
			//		line_heat[col];
			//		aplha_data[col];
			//		this->_colorList[aplha_data[col]];
			//		this->line_heat[col] = this->_colorList[aplha_data[col]];//取出的颜色再设置到_heatImg对应的地址
			//	}
			//}
		}
		//qDebug() << "18->!";//1！-2！：1ms
		//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822
		//在源图上画热力图
		{
			//qDebug() << "19->!";//1！-2！：1ms
			//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822
			QPainter p(&src);
			//qDebug() << "20->!";//1！-2！：1ms
			//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822

			//p.setPen(Qt::red);

			QRadialGradient gradient(position.posX, position.posY, position.radius);
			gradient.setColorAt(0, QColor(Qt::red));//255->alpha,at(0)是最中心的位置
			gradient.setColorAt(1, QColor(Qt::red));//at(1)是以radius为半径画圆的范围内，最边缘的位置
			p.setBrush(gradient);
			p.drawEllipse(QPointF(position.posX, position.posY), position.radius, position.radius);

			//绘制热力图
			//p.drawImage(0, 0, *this->_pHotImg);//60ms
			//p.drawPixmap(src.rect(), QPixmap::fromImage(*this->_pHotImg), src.rect());//60ms
			//qDebug() << this->_pHotImg->save("_pHotImg.jpg","jpg", 100);

			//test
			//p.drawImage(0, 0, src);//

			//qDebug() << "21->!";//1！-2！：1ms
			//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822
		}
		//qDebug() << "22->!";//1！-2！：1ms
		//qDebug() << QDateTime::currentMSecsSinceEpoch();//
	};


#endif //CUSTOMHM_HPP




};

