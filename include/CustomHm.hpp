/**
* function:drawCloudMap --���ã�������
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
};//Point �ṹ������������widget�ľ���λ�ã��ͻ���Բ�İ뾶�Լ�������Ĵ������������������Ȩ�ؼ���ı���
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
	float mAspectRatio;//���
	float mHighRadio;//�߱�
	//alphaͼ
	QImage* _pAlphaImg = nullptr;
	//hotMap
	QImage* _pHotImg = nullptr;

protected:
	QSize mImageSize;
	//�����͸����[0-255]
	static constexpr int HeatAlpha = 200;
	//��ɫ��͸����Ϊ0
	QRgb _colorList[256];
	//ת����ͼ
	QRgb* line_heat = nullptr;

public:
	//����̬��
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
	//������ͼȡɫ�б�
	void SetColorList()
	{
		//�������Խ���ɫ���õ���ɫ��
		QLinearGradient linear = QLinearGradient(QPoint(0, 0), QPoint(255, 0));//����ɫ��
		linear.setColorAt(0, Qt::blue);
		linear.setColorAt(0.4, Qt::blue);//��ɫ
		linear.setColorAt(0.5, Qt::cyan);//����ɫ
		linear.setColorAt(0.6, Qt::green);//��ɫ
		linear.setColorAt(0.8, Qt::yellow);//��ɫ
		linear.setColorAt(0.95, Qt::red);//��ɫ

		//�ѽ���ɫ���Ƶ�Img����ȡ��ɫ
		QImage img(256, 1, QImage::Format_ARGB32);//����һ��QImage,���ڻ��ƽ���ɫ
		//img = QImage(256, 1, QImage::Format_ARGB32);//����һ��QImage,���ڻ��ƽ���ɫ
		QPainter painter(&img);
		painter.fillRect(img.rect(), linear);//������ɫ�����Ƶ����QImage��

		//HeatAlphaΪ����ͼ����͸����
		quint32 alpha = 0;

		for (quint32 i = 0; i < 256; i++) {
			//��������ͼ͸������������ɫ���͸����
			alpha = HeatAlpha / 255.0 * i;	//HeatAlpha : 200;aplha:[0,200]

			//typedef unsigned int QRgb: format #AARRGGBB
			//��ɫ+͸���� _colorList����ɫ��
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
			this->_pAlphaImg->fill(Qt::transparent);//�ڵĵĵ���alpha��͸����
		}
		if (this->_pHotImg == nullptr)
		{
			this->_pHotImg = new QImage(src.size(), QImage::Format_ARGB32);
			this->_pHotImg->fill(Qt::transparent);//�ڵĵĵ���alpha��͸����
		}

		//������ͼ��͸��ͼ���
		this->ResetAphalHotmap();

		//��͸��ͼ
		{
			QPainter painter(this->_pAlphaImg);
			painter.setPen(Qt::transparent);

			QRadialGradient gradient(position.posX, position.posY, position.radius);
			gradient.setColorAt(0, QColor(0, 0, 0, 255));//255->alpha,at(0)�������ĵ�λ��
			gradient.setColorAt(1, QColor(0, 0, 0, 100));//at(1)����radiusΪ�뾶��Բ�ķ�Χ�ڣ����Ե��λ��
			painter.setBrush(gradient);
			painter.drawEllipse(QPointF(position.posX, position.posY), position.radius, position.radius);
		}
		//͸����ת��ͼ
		{
			//��alphaֵתΪ��ɫֵ
			for (int row = 0; row < this->_pAlphaImg->height(); row++)
			{
				//dataimg QImage::Format_Alpha8��һ����1���ֽ�
				const uchar* aplha_data = this->_pAlphaImg->scanLine(row);//��ȡ͸��ͼ�ϵ�ɨ���ߣ���700����

				//heatimg QImage::Format_ARGB32��һ����4���ֽ�
				this->line_heat = reinterpret_cast<QRgb*>(this->_pHotImg->scanLine(row));//��ȡ����ͼ�ϵ�ɨ���ߣ���700�С�


				for (int col = 0; col < this->_pAlphaImg->width(); col++)//��ÿ�е�ɨ����ȡ��λ������1000��
				{
					//����alpha͸���ȴ���ɫ��ȡ��ɫ
					line_heat[col];
					aplha_data[col];
					this->_colorList[aplha_data[col]];
					this->line_heat[col] = this->_colorList[aplha_data[col]];//ȡ������ɫ�����õ�_heatImg��Ӧ�ĵ�ַ
				}
			}
		}

		//��Դͼ�ϻ�����ͼ
		{
			QPainter p(&src);
			//��������ͼ
			p.drawImage(0, 0, *this->_pHotImg);
			//qDebug() << this->_pHotImg->save("_pHotImg.jpg","jpg", 100);
		}

	};

/**
* \core this overload function 
*/
	void DrawCloudMap(QImage& src, const MyPoint position,QMutex& mutex)
	{
		//qDebug() << "11->!";//1��-2����1ms
		//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822
		QMutexLocker mlocker(&this->mMutex);//�ڲ���
		//qDebug() << "12->!";//1��-2����1ms
		//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822
		QMutexLocker mOutlocker(&mutex);//�ⲿ��
		//qDebug() << "13->!";//1��-2����1ms
		//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822

		this->mImageSize = src.size();

		//qDebug() << "14->!";//1��-2����1ms
		//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822
		if (this->_pAlphaImg == nullptr)
		{
			this->_pAlphaImg = new QImage(src.size(), QImage::Format_Alpha8);
			this->_pAlphaImg->fill(Qt::transparent);//�ڵĵĵ���alpha��͸����
		}
		if (this->_pHotImg == nullptr)
		{
			this->_pHotImg = new QImage(src.size(), QImage::Format_ARGB32);
			this->_pHotImg->fill(Qt::transparent);//�ڵĵĵ���alpha��͸����
		}
		//qDebug() << "15->!";//1��-2����1ms
		//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822

		//������ͼ��͸��ͼ���
		this->ResetAphalHotmap();//3ms

		//qDebug() << "16->!";//1��-2����1ms
		//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822
		//��͸��ͼ
		{
			//QPainter painter(this->_pAlphaImg);
			//painter.setPen(Qt::transparent);

			//QRadialGradient gradient(position.posX, position.posY, position.radius);
			//gradient.setColorAt(0, QColor(0, 0, 0, 255));//255->alpha,at(0)�������ĵ�λ��
			//gradient.setColorAt(1, QColor(0, 0, 0, 100));//at(1)����radiusΪ�뾶��Բ�ķ�Χ�ڣ����Ե��λ��
			//painter.setBrush(gradient);
			//painter.drawEllipse(QPointF(position.posX, position.posY), position.radius, position.radius);
		}
		//qDebug() << "17->!";//1��-2����1ms
		//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822
		//͸����ת��ͼ
		{
			////��alphaֵתΪ��ɫֵ
			//for (int row = 0; row < this->_pAlphaImg->height(); row++)
			//{
			//	//dataimg QImage::Format_Alpha8��һ����1���ֽ�
			//	const uchar* aplha_data = this->_pAlphaImg->scanLine(row);//��ȡ͸��ͼ�ϵ�ɨ���ߣ���700����

			//	//heatimg QImage::Format_ARGB32��һ����4���ֽ�
			//	this->line_heat = reinterpret_cast<QRgb*>(this->_pHotImg->scanLine(row));//��ȡ����ͼ�ϵ�ɨ���ߣ���700�С�


			//	for (int col = 0; col < this->_pAlphaImg->width(); col++)//��ÿ�е�ɨ����ȡ��λ������1000��
			//	{
			//		//����alpha͸���ȴ���ɫ��ȡ��ɫ
			//		line_heat[col];
			//		aplha_data[col];
			//		this->_colorList[aplha_data[col]];
			//		this->line_heat[col] = this->_colorList[aplha_data[col]];//ȡ������ɫ�����õ�_heatImg��Ӧ�ĵ�ַ
			//	}
			//}
		}
		//qDebug() << "18->!";//1��-2����1ms
		//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822
		//��Դͼ�ϻ�����ͼ
		{
			//qDebug() << "19->!";//1��-2����1ms
			//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822
			QPainter p(&src);
			//qDebug() << "20->!";//1��-2����1ms
			//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822

			//p.setPen(Qt::red);

			QRadialGradient gradient(position.posX, position.posY, position.radius);
			gradient.setColorAt(0, QColor(Qt::red));//255->alpha,at(0)�������ĵ�λ��
			gradient.setColorAt(1, QColor(Qt::red));//at(1)����radiusΪ�뾶��Բ�ķ�Χ�ڣ����Ե��λ��
			p.setBrush(gradient);
			p.drawEllipse(QPointF(position.posX, position.posY), position.radius, position.radius);

			//��������ͼ
			//p.drawImage(0, 0, *this->_pHotImg);//60ms
			//p.drawPixmap(src.rect(), QPixmap::fromImage(*this->_pHotImg), src.rect());//60ms
			//qDebug() << this->_pHotImg->save("_pHotImg.jpg","jpg", 100);

			//test
			//p.drawImage(0, 0, src);//

			//qDebug() << "21->!";//1��-2����1ms
			//qDebug() << QDateTime::currentMSecsSinceEpoch();//1685351203 822
		}
		//qDebug() << "22->!";//1��-2����1ms
		//qDebug() << QDateTime::currentMSecsSinceEpoch();//
	};


#endif //CUSTOMHM_HPP




};

