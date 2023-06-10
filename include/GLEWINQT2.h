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
    *   绘制函数
    */
    virtual void    render();//渲染
    void generateVertices();//计算球的顶点坐标
    void generateIndices();//计算球顶点的下标
    void generatSphere();//球
    void generateMatrixs();//构建旋转、透视（透视）、频移矩阵。
    void generateCircle();//构建圆形数据
    
public:
	//声源位置
	QPointF m_soundLocation;
    //render
    QTimer* _pRenderTimer = nullptr;
protected:
    GLContext   _context; //这个窗口的上下文

    ObServerationAngle m_Observer;
private:
    std::vector<float> sphereVertices;//顶点
    std::vector<int> sphereIndices;//下标
    // 大球
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

    //小球位置
    float posit[3];

    //program2 成员
    GLuint vao;
    GLuint vbo[2];
    GLuint mvLocTwo, projLocTwo, costColorTwo;

private:
    Ui::GLEWINQT2Class *ui;
};
