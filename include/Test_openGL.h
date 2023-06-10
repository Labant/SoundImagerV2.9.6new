#pragma once

#include <QtWidgets/QWidget>
//#include "ui_Test_openGL.h"
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include "Camera.h"
#include <QMouseEvent>
#include "ObServerationAngle.h"
#include <QPointF>

QT_BEGIN_NAMESPACE
namespace Ui { class Test_openGLClass; };
QT_END_NAMESPACE

#define longitude 180.0 //经度
#define latitude 360.0 //纬度
struct Vertex
{
	QVector3D pos;
	QVector2D texture;
	QVector3D normal;
};


class Test_openGL : public QOpenGLWidget, public QOpenGLExtraFunctions
{
    Q_OBJECT

public:
    Test_openGL(QWidget *parent = nullptr);
    ~Test_openGL();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int w,int h) override;
    virtual void paintGL() override;
    virtual void timerEvent(QTimerEvent* event);
    virtual void enterEvent(QEvent* event);
    virtual void leaveEvent(QEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    void sherePos(int y,int x);
private:
    Ui::Test_openGLClass *ui;
    //球
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;
    QOpenGLShaderProgram* m_program = nullptr;
    QOpenGLTexture* m_texture = nullptr;
    //点
	//QOpenGLVertexArrayObject m_vao_point;
	//QOpenGLBuffer m_vbo_point;
	QOpenGLShaderProgram* m_program_point = nullptr;

private:
	QMatrix4x4 m_projection;//投影矩阵负责让你看到的画面，符合近大远小的透视规律
	QMatrix4x4 m_view;//视图矩阵负责模拟一个摄像机的镜头，让你可以在3D空间中的某个位置去观察另一个位置
	QMatrix4x4 m_model;//模型矩阵负责让你绘制模型的时候，可以顺便对它进行缩放，旋转，平移的操作
    int m_angle;
	Camera m_camera;
    ObServerationAngle m_Observer;

	float* m_vertexBuffer = nullptr;
	int m_vertexCount = 0;
	int m_row = 0;
	int m_col = 0;
    QVector<Vertex> m_vertices;
    QVector<QVector<QVector3D>> _vertexMatrix;

    QVector3D _p0;

    //移动的初始位置
    QPoint m_posStart;
	//移动中的位置
	QPoint m_pomoving;

    //点位置位于球的坐标
    int m_sherePos_X = 0;
    int m_sherePos_Y = 0;

    //精度
    int m_presicionX = 0;
    int m_presicionY = 0;
public:
	//声源位置
	QPoint m_soundLocation;

};

