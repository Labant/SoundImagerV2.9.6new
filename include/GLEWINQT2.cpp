#include "GLEWINQT2.h"
#include <QTimer>
#include "GLContext.h"
#include "glCheck.hpp"
#include <QDebug>
#include <math.h>

const GLfloat  PI = 3.14159265358979323846f;

//将球横纵划分成50X50的网格
const int Y_SEGMENTS = 50;
const int X_SEGMENTS = 50;
//const int Y_SEGMENTS = 180;
//const int X_SEGMENTS = 360;

GLEWINQT2::GLEWINQT2(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    setAutoFillBackground(false);

    // init opengl
    _context.setup((HWND)winId(), GetDC((HWND)winId()));
    // setup glew
    if (glewInit() != GLEW_OK)
    {
        qDebug() << "glewInit failed";
    }


    //ui->setupUi(this);

    //floor() //下取整
    //ceil()  //上取
	//圆形数据
	this->generateCircle();

    //1、生成顶点
    this->generateVertices();

    //2、生成下标
    this->generateIndices();

    //3、生成球体
    this->generatSphere();

	//render
	_pRenderTimer = new QTimer(this);
    QObject::connect(_pRenderTimer, SIGNAL(timeout()), this, SLOT(render()));
	_pRenderTimer->setInterval(0);
	_pRenderTimer->start();



	//被子类监视
	installEventFilter(&m_Observer);

}

GLEWINQT2::~GLEWINQT2()
{
    _context.shutdown();
    //delete ui;
}

void GLEWINQT2::paintEvent(QPaintEvent* evt)
{
}
void GLEWINQT2::generateVertices()
{
    // 生成球的顶点
    for (int y = 0; y <= Y_SEGMENTS; y++)
    {
        for (int x = 0; x <= X_SEGMENTS; x++)
        {
            float xSegment = (float)x / (float)X_SEGMENTS;
            float ySegment = (float)y / (float)Y_SEGMENTS;
            float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            float yPos = std::cos(ySegment * PI);
            float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);//xSegment * 2.0f * PI = a~f角;ySegment * PI= beita,
            //原理：https://blog.csdn.net/qq_51701007/article/details/125560742

            sphereVertices.push_back(xPos);
            sphereVertices.push_back(yPos);
            sphereVertices.push_back(zPos);
        }
    }
}
void GLEWINQT2::generateIndices()
{
    // 生成球的Indices(数据下标索引)
    for (int i = 0; i < Y_SEGMENTS; i++)
    {
        for (int j = 0; j < X_SEGMENTS; j++)
        {

            sphereIndices.push_back(i * (X_SEGMENTS + 1) + j);
            sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j);
            sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);

            sphereIndices.push_back(i * (X_SEGMENTS + 1) + j);
            sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
            sphereIndices.push_back(i * (X_SEGMENTS + 1) + j + 1);
        }
    }
}
void GLEWINQT2::generatSphere()
{

    //创建 program  大球 
    renderingProgram = glCheck::createShaderProgram("glsl/vertShader.glsl", "glsl/fragShader.glsl");

    //观察角度
    cameraX = 0.0f; cameraY = 0.0f; cameraZ = 2.2f;

    glGenVertexArrays(1, &VAO);
    //生成并绑定球体的VAO和VBO
    glBindVertexArray(VAO);
    glGenBuffers(2, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

    // 将顶点数据绑定至当前 vbo中的第一个缓存区
    glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), &sphereVertices[0], GL_STATIC_DRAW);

    GLuint element_buffer_object; //EBO
    glGenBuffers(1, &element_buffer_object);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(int), &sphereIndices[0], GL_STATIC_DRAW);

    // 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //// 解绑VAO和VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


 //   //！方案一：画圆 program2
 //   renderingProgramTwo = glCheck::createShaderProgram("glsl/vertShader2.glsl", "glsl/fragShader2.glsl");
 //   //创建vao
 //   glGenVertexArrays(1, &vao);
 //   //激活vao
 //   glBindVertexArray(vao);
 //   //创建vbo
 //   glGenBuffers(1, &vbo[0]);
 //   //激活vbo
 //   glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
 //   //vbo填充数据
 //   glBufferData(GL_ARRAY_BUFFER, sizeof(posit), posit, GL_STATIC_DRAW);
 //   //绑定属性指针
 //   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
 //   //启用属性指针
 //   glEnableVertexAttribArray(0);
 //   //解绑vao vbo
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);



	//！方案一：画圆 program2
	//激活vbo
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	//vbo填充数据
	glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), &sphereVertices[0], GL_STATIC_DRAW);

	GLuint element_buffer_object2; //EBO
	glGenBuffers(1, &element_buffer_object2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(int), &sphereIndices[0], GL_STATIC_DRAW);

	//绑定属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	//启用属性指针
	glEnableVertexAttribArray(0);
	//解绑vao vbo
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void GLEWINQT2::generateMatrixs()
{
	//构建透视矩阵
	aspect = (float)width() / (float)height();//横纵比
	pMat = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 100.0f);//1.0472 radians = 60 °

	//构建平移矩阵
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-(cameraX), -(cameraY), -(cameraZ)));

	//构建旋转矩阵
	//static long long xx = QDateTime::currentSecsSinceEpoch();
	//float cc = xx - QDateTime::currentSecsSinceEpoch();
 //   qDebug() << "CC:" << cc;
    //cc = -360;
    //m_Observer.m_pomoving;
    //qDebug() << "m_Observer.m_pomoving.x():" << m_Observer.m_pomoving.y();
	//围绕X轴旋转 所需移动的y值
    //if (m_Observer.isMove)qDebug() << "(float)abs(m_Observer.m_pomoving.y():" << (float)abs(m_Observer.m_pomoving.y());

    //m_Observer.m_pomoving.y() xx_c += 1
    static float xx_c = 1.0f;
    //qDebug() << "m_Observer.m_posMany.y():" << m_Observer.m_posMany.y();
	//qDebug() << "glm::radians((float)m_Observer.m_posMany.y()):" << glm::radians((float)m_Observer.m_posMany.y());
	//qDebug() << "glm::radians(157.0f):" << glm::radians(157.0f);// 3.14159
	//qDebug() << "glm::radians(-157.0f):" << glm::radians(-157.0f);// 3.14159
	//rotate函数，第二参数为弧度制 *0.02 用以控制速度 (float)m_Observer.m_posMany.y()
    rMat = glm::rotate(glm::mat4(1.0f), 0.02f * glm::radians((float)m_Observer.m_posMany.y()), glm::vec3(1.0f, 0.0f, 0.0f));

	//裁剪 +60->0 和 360 - 300
	//加个旋转便于观察
	rMat = glm::rotate(rMat, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	
    //围绕Y轴旋转 所需移动的x值
	//rMat = glm::rotate(rMat, 0.1f * (float)cc, glm::vec3(0.0f, 1.0f, 0.0f));// 0.2f * (float)cc

    mMat = rMat;

	//mMat = glm::mat4(1.0f);

    //构建mv矩阵，shader里只声明了mv矩阵和投影矩阵
	mvMat = vMat * mMat;
}

void GLEWINQT2::generateCircle()
{
	//for (int i = 0; i < circle_decomposition; i++)
	//{
 //       //circleData.push_back(r_circle * cos(2 * PI * i / circle_decomposition));
 //       //circleData.push_back(r_circle * sin(2 * PI * i / circle_decomposition));
	//	//glVertex2f(r_circle * cos(2 * PI * i / circle_decomposition), r_circle * sin(2 * PI * i / circle_decomposition));   //定义顶点
	//}
}

void GLEWINQT2::render()
{
    //大小适应
    glViewport(0, 0, width(), height());

    //清除颜色缓存区所用的颜色
    glClearColor(0, 0.5, 0.7, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//绘制球
    ////开启面剔除(只需要展示一个面，否则会有重合)
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//开启深度测试。近处可以挡住远处物体
	glEnable(GL_DEPTH_TEST);
	//使用线框模式绘制
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //绑定VAO
    glBindVertexArray(VAO);


    //使用program 大球
    glUseProgram(renderingProgram);

    //获取mv矩阵和投影矩阵的统一变量
    mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
    projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
    costColor = glGetUniformLocation(renderingProgram, "costomColor");

    //组织透视、频移、旋转矩阵
    generateMatrixs();

    //将透视矩阵和mv矩阵复制给相应的统一变量
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
    //颜色
    glUniform4f(costColor,0.0f,0.0f,0.0f,1.0f);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    // 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glDrawElements(GL_TRIANGLES, X_SEGMENTS * Y_SEGMENTS * 6, GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_TRIANGLES, 0, X_SEGMENTS * Y_SEGMENTS * 6);

 //   // 解绑VAO和VBO 方案二不解绑
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

    //重新组织点缓冲区数据
 //   static float x_ = 50.0f / 360.0f;//0.138
	//static float y_ = 152.0f / 180.0f;//0.844
	static float x_ = 51.0f / 360.0f;//0.138 //x轴每圈格子
	static float y_ = 51.0f / 180.0f;//0.844 //y轴每圈格子

    //原理：每一圈都是51数据点，y轴位置为：（y-1）*51
    float x_y = floor(m_soundLocation.x() * x_) * 3.0f + (floor(m_soundLocation.y() * y_) * 51.0f * 3.0f);

    //qDebug() << "m_soundLocation:" << m_soundLocation;

	//x:90;y:90;
	posit[0] = sphereVertices[x_y + 0];
	posit[1] = sphereVertices[x_y + 1];
	posit[2] = sphereVertices[x_y + 2];



    //！方案一：画点
	////激活vbo
	//glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	////vbo填充数据
	//glBufferData(GL_ARRAY_BUFFER, sizeof(posit), posit, GL_STATIC_DRAW);
 //   glBindBuffer(GL_ARRAY_BUFFER,0);

	////program2 小球
	//glUseProgram(renderingProgramTwo);

	////获取mv矩阵和投影矩阵的统一变量
	//mvLocTwo = glGetUniformLocation(renderingProgramTwo, "mv_matrix2");
	//projLocTwo = glGetUniformLocation(renderingProgramTwo, "proj_matrix2");
	//costColorTwo = glGetUniformLocation(renderingProgramTwo, "costomColor2");

	////将透视矩阵和mv矩阵复制给相应的统一变量
	//glUniformMatrix4fv(mvLocTwo, 1, GL_FALSE, glm::value_ptr(mvMat));
	//glUniformMatrix4fv(projLocTwo, 1, GL_FALSE, glm::value_ptr(pMat));
	//glUniform4f(costColorTwo, 1.0f, 0, 0.0f, 1.0f);

	////激活缓存区
	//glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	////设置顶点属性指针
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	////点的大小
	//glPointSize(50);
	//glDrawArrays(GL_POINTS, 0, 1);

	//// 解绑VAO和VBO
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);


    //！方案二：画小球
		//绘制球
	////开启面剔除(只需要展示一个面，否则会有重合)
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//开启深度测试。近处可以挡住远处物体
	glEnable(GL_DEPTH_TEST);
	////使用线框模式绘制
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//激活vbo
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	//vbo填充数据
	//glBufferData(GL_ARRAY_BUFFER, sizeof(posit), posit, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 1);

	//program2 小球
	//glUseProgram(renderingProgramTwo);

	//获取mv矩阵和投影矩阵的统一变量
	mvLocTwo = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLocTwo = glGetUniformLocation(renderingProgram, "proj_matrix");
	costColorTwo = glGetUniformLocation(renderingProgram, "costomColor");

	//test
	/**分析：
	 * 上下移动(Y轴)：
	 * 上顶点：(0,1,0)对应(0,0)
	 * 下顶点：(0,-1,0)对应(0,180)
	 *  其他需加上z轴 如(0,90)  z_p:2/90 = 0.0222f;
	 */
	//mvMat *= glm::translate(glm::mat4(1.0f), glm::vec3(0, -1, 0));//tMat (0,180）
	//mvMat *= glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 1));//tMat (0,90）
	mvMat *= glm::translate(glm::mat4(1.0f), glm::vec3(posit[0], posit[1], posit[2]));//tMat (0,90）
	//mvMat *= glm::rotate(glm::mat4(1.0f), 0.01f * (float)m_Observer.m_posMany.y(), glm::vec3(0, 1, 0));//rMat 小球不需要旋转
	mvMat *= glm::scale(glm::mat4(1.0f), glm::vec3(0.05, 0.05, 0.05));//sMat

	//将透视矩阵和mv矩阵复制给相应的统一变量
	glUniformMatrix4fv(mvLocTwo, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLocTwo, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniform4f(costColorTwo, 1.0f, 0, 0.0f, 1.0f);

	//激活缓存区 同大球数据一样
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	//设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//画 小球体
	glDrawElements(GL_TRIANGLES, X_SEGMENTS* Y_SEGMENTS * 6, GL_UNSIGNED_INT, 0);

	// 解绑VAO和VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

    _context.swapBuffer();
}