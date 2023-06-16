#pragma once

#include <QtWidgets/QWidget>
#include "GLContext.h"
#include <QTimer>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <QDateTime>
#include <QPoint>
#include "ObServerationAngle.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GLEWINQT2Class; };
QT_END_NAMESPACE

class GLEWINQT2 : public QWidget
{
    Q_OBJECT

public:
    GLEWINQT2(QWidget *parent = nullptr);
    ~GLEWINQT2();


protected:
    void    paintEvent(QPaintEvent* evt);
public Q_SLOTS:
    /**
    *   ���ƺ���
    */
    virtual void    render();//��Ⱦ
    void generateVertices();//������Ķ�������
    void generateIndices();//�����򶥵���±�
    void generatSphere();//��
    void generateMatrixs();//������ת��͸�ӣ�͸�ӣ���Ƶ�ƾ���
    void generateCircle();//����Բ������
    
public:
	//��Դλ��
	QPointF m_soundLocation;
    //render
    QTimer* _pRenderTimer = nullptr;
protected:
    GLContext   _context; //������ڵ�������

    ObServerationAngle m_Observer;
private:
    std::vector<float> sphereVertices;//����
    std::vector<int> sphereIndices;//�±�
    // ����
    GLuint VBO[2], VAO;

    //2.
    GLuint renderingProgram, renderingProgramTwo;

    GLuint mvLoc, projLoc,costColor;
    GLuint position2, customColor2;
    GLuint positionCircle, customColorCircle;
    float aspect;
    glm::mat4 pMat, vMat, mMat, mvMat;
    float cameraX, cameraY, cameraZ;
    glm::mat4 tMat, rMat;

    //С��λ��
    float posit[3];

    //program2 ��Ա
    GLuint vao;
    GLuint vbo[2];
    GLuint mvLocTwo, projLocTwo, costColorTwo;

private:
    Ui::GLEWINQT2Class *ui;
};
