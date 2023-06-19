#ifndef LVSDRAWGRADIENTROUND_HPP
#define LVSDRAWGRADIENTROUND_HPP
#include <QObject>
#include <QPainter>
#include <QBrush>
#include <QImage>
#include <QRadialGradient>
#include <QMutex>
#include "GlobalConfig.hpp"

class LvsDrawGradientRound:QObject
{

private:
    int radius = 120;

public:
    explicit LvsDrawGradientRound(){};
    ~LvsDrawGradientRound(){};

/**
 * \core function
 */
	void DrawGradientRound(QImage& src, QPointF pos)
	{

		QPainter mPainter(&src);
		mPainter.setRenderHint(QPainter::Antialiasing, true);


		QRadialGradient mRadialGradient(pos.rx(), pos.ry(), this->radius, pos.rx(), pos.ry());
		mRadialGradient.setColorAt(0, Qt::blue);
		mRadialGradient.setColorAt(0.4, Qt::blue);//��ɫ
		mRadialGradient.setColorAt(0.5, Qt::cyan);//����ɫ
		mRadialGradient.setColorAt(0.6, Qt::green);//��ɫ
		mRadialGradient.setColorAt(0.8, Qt::yellow);//��ɫ
		mRadialGradient.setColorAt(0.95, Qt::red);//��ɫ

		mPainter.setPen(Qt::NoPen);
		mPainter.setBrush(QBrush(mRadialGradient));
		mPainter.drawEllipse(pos.rx() - this->radius, pos.ry() - this->radius, this->radius << 1, this->radius << 1);

	};

/**
* \core this overload function
*/
	void DrawGradientRound(QImage& src, QPointF pos ,QMutex& mutex)
	{
		QMutexLocker mMutexLocker(&mutex);

		QPainter mPainter(&src);
		mPainter.setRenderHint(QPainter::Antialiasing, true);


		QRadialGradient mRadialGradient(pos.rx(), pos.ry(), this->radius, pos.rx(), pos.ry());
		mRadialGradient.setColorAt(0, Qt::blue);
		mRadialGradient.setColorAt(0.4, Qt::blue);//��ɫ
		mRadialGradient.setColorAt(0.5, Qt::cyan);//����ɫ
		mRadialGradient.setColorAt(0.6, Qt::green);//��ɫ
		mRadialGradient.setColorAt(0.8, Qt::yellow);//��ɫ
		mRadialGradient.setColorAt(0.95, Qt::red);//��ɫ

		mPainter.setPen(Qt::NoPen);
		mPainter.setBrush(QBrush(mRadialGradient));
		mPainter.drawEllipse(pos.rx() - this->radius, pos.ry() - this->radius, this->radius << 1, this->radius << 1);

	};

	/**
	* \core this overload function
	*/
	void DrawGradientRound(QImage& src, MyPoint pos, QMutex& mutex)
	{
		QMutexLocker mMutexLocker(&mutex);

		QPainter mPainter(&src);
		mPainter.setRenderHint(QPainter::Antialiasing, true);


		QRadialGradient mRadialGradient(pos.posX, pos.posY, pos.radius, pos.posX, pos.posY);
		//mRadialGradient.setColorAt(0, Qt::blue);
		//mRadialGradient.setColorAt(0.4, Qt::blue);//��ɫ
		//mRadialGradient.setColorAt(0.5, Qt::cyan);//����ɫ
		//mRadialGradient.setColorAt(0.6, Qt::green);//��ɫ
		//mRadialGradient.setColorAt(0.8, Qt::yellow);//��ɫ
		//mRadialGradient.setColorAt(0.95, Qt::red);//��ɫ

		mRadialGradient.setColorAt(0.95, QColor(0, 0, 255,80));
		mRadialGradient.setColorAt(0.8, QColor(0, 0, 255, 80));//��ɫ
		mRadialGradient.setColorAt(0.6, QColor(102, 204, 255,90));//����ɫ
		mRadialGradient.setColorAt(0.5, QColor(0, 255, 0,120));//��ɫ
		mRadialGradient.setColorAt(0.4, QColor(255, 255, 0,150));//��ɫ
		mRadialGradient.setColorAt(0, QColor(255,0,0,180));//��ɫ //������

		mPainter.setPen(Qt::NoPen);
		mPainter.setBrush(QBrush(mRadialGradient));
		mPainter.drawEllipse(pos.posX - pos.radius, pos.posY - pos.radius, pos.radius << 1, pos.radius << 1);

	};


 //!setting
    void setRadius(int R)
    {
        this->radius = R;
    };
};



#endif
