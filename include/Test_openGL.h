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

#define longitude 180.0 //����
#define latitude 360.0 //γ��
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
    //��
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;
    QOpenGLShaderProgram* m_program = nullptr;
    QOpenGLTexture* m_texture = nullptr;
    //��
	//QOpenGLVertexArrayObject m_vao_point;
	//QOpenGLBuffer m_vbo_point;
	QOpenGLShaderProgram* m_program_point = nullptr;

private:
	QMatrix4x4 m_projection;//ͶӰ���������㿴���Ļ��棬���Ͻ���ԶС��͸�ӹ���
	QMatrix4x4 m_view;//��ͼ������ģ��һ��������ľ�ͷ�����������3D�ռ��е�ĳ��λ��ȥ�۲���һ��λ��
	QMatrix4x4 m_model;//ģ�;������������ģ�͵�ʱ�򣬿���˳������������ţ���ת��ƽ�ƵĲ���
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

    //�ƶ��ĳ�ʼλ��
    QPoint m_posStart;
	//�ƶ��е�λ��
	QPoint m_pomoving;

    //��λ��λ���������
    int m_sherePos_X = 0;
    int m_sherePos_Y = 0;

    //����
    int m_presicionX = 0;
    int m_presicionY = 0;
public:
	//��Դλ��
	QPoint m_soundLocation;

};

