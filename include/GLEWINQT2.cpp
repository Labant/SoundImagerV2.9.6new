#include "GLEWINQT2.h"
#include <QTimer>
#include "GLContext.h"
#include "glCheck.hpp"
#include <QDebug>
#include <math.h>

const GLfloat  PI = 3.14159265358979323846f;

//������ݻ��ֳ�50X50������
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

    //floor() //��ȡ��
    //ceil()  //��ȡ
	//Բ������
	this->generateCircle();

    //1�����ɶ���
    this->generateVertices();

    //2�������±�
    this->generateIndices();

    //3����������
    this->generatSphere();

	//render
	_pRenderTimer = new QTimer(this);
    QObject::connect(_pRenderTimer, SIGNAL(timeout()), this, SLOT(render()));
	_pRenderTimer->setInterval(0);
	_pRenderTimer->start();



	//���������
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
    // ������Ķ���
    for (int y = 0; y <= Y_SEGMENTS; y++)
    {
        for (int x = 0; x <= X_SEGMENTS; x++)
        {
            float xSegment = (float)x / (float)X_SEGMENTS;
            float ySegment = (float)y / (float)Y_SEGMENTS;
            float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            float yPos = std::cos(ySegment * PI);
            float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);//xSegment * 2.0f * PI = a~f��;ySegment * PI= beita,
            //ԭ��https://blog.csdn.net/qq_51701007/article/details/125560742

            sphereVertices.push_back(xPos);
            sphereVertices.push_back(yPos);
            sphereVertices.push_back(zPos);
        }
    }
}
void GLEWINQT2::generateIndices()
{
    // �������Indices(�����±�����)
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

    //���� program  ���� 
    renderingProgram = glCheck::createShaderProgram("glsl/vertShader.glsl", "glsl/fragShader.glsl");

    //�۲�Ƕ�
    cameraX = 0.0f; cameraY = 0.0f; cameraZ = 2.2f;

    glGenVertexArrays(1, &VAO);
    //���ɲ��������VAO��VBO
    glBindVertexArray(VAO);
    glGenBuffers(2, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

    // ���������ݰ�����ǰ vbo�еĵ�һ��������
    glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), &sphereVertices[0], GL_STATIC_DRAW);

    GLuint element_buffer_object; //EBO
    glGenBuffers(1, &element_buffer_object);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(int), &sphereIndices[0], GL_STATIC_DRAW);

    // ���ö�������ָ��
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //// ���VAO��VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


 //   //������һ����Բ program2
 //   renderingProgramTwo = glCheck::createShaderProgram("glsl/vertShader2.glsl", "glsl/fragShader2.glsl");
 //   //����vao
 //   glGenVertexArrays(1, &vao);
 //   //����vao
 //   glBindVertexArray(vao);
 //   //����vbo
 //   glGenBuffers(1, &vbo[0]);
 //   //����vbo
 //   glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
 //   //vbo�������
 //   glBufferData(GL_ARRAY_BUFFER, sizeof(posit), posit, GL_STATIC_DRAW);
 //   //������ָ��
 //   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
 //   //��������ָ��
 //   glEnableVertexAttribArray(0);
 //   //���vao vbo
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);



	//������һ����Բ program2
	//����vbo
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	//vbo�������
	glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), &sphereVertices[0], GL_STATIC_DRAW);

	GLuint element_buffer_object2; //EBO
	glGenBuffers(1, &element_buffer_object2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(int), &sphereIndices[0], GL_STATIC_DRAW);

	//������ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	//��������ָ��
	glEnableVertexAttribArray(0);
	//���vao vbo
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void GLEWINQT2::generateMatrixs()
{
	//����͸�Ӿ���
	aspect = (float)width() / (float)height();//���ݱ�
	pMat = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 100.0f);//1.0472 radians = 60 ��

	//����ƽ�ƾ���
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-(cameraX), -(cameraY), -(cameraZ)));

	//������ת����
	//static long long xx = QDateTime::currentSecsSinceEpoch();
	//float cc = xx - QDateTime::currentSecsSinceEpoch();
 //   qDebug() << "CC:" << cc;
    //cc = -360;
    //m_Observer.m_pomoving;
    //qDebug() << "m_Observer.m_pomoving.x():" << m_Observer.m_pomoving.y();
	//Χ��X����ת �����ƶ���yֵ
    //if (m_Observer.isMove)qDebug() << "(float)abs(m_Observer.m_pomoving.y():" << (float)abs(m_Observer.m_pomoving.y());

    //m_Observer.m_pomoving.y() xx_c += 1
    static float xx_c = 1.0f;
    //qDebug() << "m_Observer.m_posMany.y():" << m_Observer.m_posMany.y();
	//qDebug() << "glm::radians((float)m_Observer.m_posMany.y()):" << glm::radians((float)m_Observer.m_posMany.y());
	//qDebug() << "glm::radians(157.0f):" << glm::radians(157.0f);// 3.14159
	//qDebug() << "glm::radians(-157.0f):" << glm::radians(-157.0f);// 3.14159
	//rotate�������ڶ�����Ϊ������ *0.02 ���Կ����ٶ� (float)m_Observer.m_posMany.y()
    rMat = glm::rotate(glm::mat4(1.0f), 0.02f * glm::radians((float)m_Observer.m_posMany.y()), glm::vec3(1.0f, 0.0f, 0.0f));

	//�ü� +60->0 �� 360 - 300
	//�Ӹ���ת���ڹ۲�
	rMat = glm::rotate(rMat, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	
    //Χ��Y����ת �����ƶ���xֵ
	//rMat = glm::rotate(rMat, 0.1f * (float)cc, glm::vec3(0.0f, 1.0f, 0.0f));// 0.2f * (float)cc

    mMat = rMat;

	//mMat = glm::mat4(1.0f);

    //����mv����shader��ֻ������mv�����ͶӰ����
	mvMat = vMat * mMat;
}

void GLEWINQT2::generateCircle()
{
	//for (int i = 0; i < circle_decomposition; i++)
	//{
 //       //circleData.push_back(r_circle * cos(2 * PI * i / circle_decomposition));
 //       //circleData.push_back(r_circle * sin(2 * PI * i / circle_decomposition));
	//	//glVertex2f(r_circle * cos(2 * PI * i / circle_decomposition), r_circle * sin(2 * PI * i / circle_decomposition));   //���嶥��
	//}
}

void GLEWINQT2::render()
{
    //��С��Ӧ
    glViewport(0, 0, width(), height());

    //�����ɫ���������õ���ɫ
    glClearColor(0, 0.5, 0.7, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//������
    ////�������޳�(ֻ��Ҫչʾһ���棬��������غ�)
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//������Ȳ��ԡ��������Ե�סԶ������
	glEnable(GL_DEPTH_TEST);
	//ʹ���߿�ģʽ����
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //��VAO
    glBindVertexArray(VAO);


    //ʹ��program ����
    glUseProgram(renderingProgram);

    //��ȡmv�����ͶӰ�����ͳһ����
    mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
    projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
    costColor = glGetUniformLocation(renderingProgram, "costomColor");

    //��֯͸�ӡ�Ƶ�ơ���ת����
    generateMatrixs();

    //��͸�Ӿ����mv�����Ƹ���Ӧ��ͳһ����
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
    //��ɫ
    glUniform4f(costColor,0.0f,0.0f,0.0f,1.0f);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    // ���ö�������ָ��
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glDrawElements(GL_TRIANGLES, X_SEGMENTS * Y_SEGMENTS * 6, GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_TRIANGLES, 0, X_SEGMENTS * Y_SEGMENTS * 6);

 //   // ���VAO��VBO �����������
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

    //������֯�㻺��������
 //   static float x_ = 50.0f / 360.0f;//0.138
	//static float y_ = 152.0f / 180.0f;//0.844
	static float x_ = 51.0f / 360.0f;//0.138 //x��ÿȦ����
	static float y_ = 51.0f / 180.0f;//0.844 //y��ÿȦ����

    //ԭ��ÿһȦ����51���ݵ㣬y��λ��Ϊ����y-1��*51
    float x_y = floor(m_soundLocation.x() * x_) * 3.0f + (floor(m_soundLocation.y() * y_) * 51.0f * 3.0f);

    //qDebug() << "m_soundLocation:" << m_soundLocation;

	//x:90;y:90;
	posit[0] = sphereVertices[x_y + 0];
	posit[1] = sphereVertices[x_y + 1];
	posit[2] = sphereVertices[x_y + 2];



    //������һ������
	////����vbo
	//glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	////vbo�������
	//glBufferData(GL_ARRAY_BUFFER, sizeof(posit), posit, GL_STATIC_DRAW);
 //   glBindBuffer(GL_ARRAY_BUFFER,0);

	////program2 С��
	//glUseProgram(renderingProgramTwo);

	////��ȡmv�����ͶӰ�����ͳһ����
	//mvLocTwo = glGetUniformLocation(renderingProgramTwo, "mv_matrix2");
	//projLocTwo = glGetUniformLocation(renderingProgramTwo, "proj_matrix2");
	//costColorTwo = glGetUniformLocation(renderingProgramTwo, "costomColor2");

	////��͸�Ӿ����mv�����Ƹ���Ӧ��ͳһ����
	//glUniformMatrix4fv(mvLocTwo, 1, GL_FALSE, glm::value_ptr(mvMat));
	//glUniformMatrix4fv(projLocTwo, 1, GL_FALSE, glm::value_ptr(pMat));
	//glUniform4f(costColorTwo, 1.0f, 0, 0.0f, 1.0f);

	////�������
	//glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	////���ö�������ָ��
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);
	////��Ĵ�С
	//glPointSize(50);
	//glDrawArrays(GL_POINTS, 0, 1);

	//// ���VAO��VBO
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);


    //������������С��
		//������
	////�������޳�(ֻ��Ҫչʾһ���棬��������غ�)
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//������Ȳ��ԡ��������Ե�סԶ������
	glEnable(GL_DEPTH_TEST);
	////ʹ���߿�ģʽ����
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//����vbo
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	//vbo�������
	//glBufferData(GL_ARRAY_BUFFER, sizeof(posit), posit, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 1);

	//program2 С��
	//glUseProgram(renderingProgramTwo);

	//��ȡmv�����ͶӰ�����ͳһ����
	mvLocTwo = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLocTwo = glGetUniformLocation(renderingProgram, "proj_matrix");
	costColorTwo = glGetUniformLocation(renderingProgram, "costomColor");

	//test
	/**������
	 * �����ƶ�(Y��)��
	 * �϶��㣺(0,1,0)��Ӧ(0,0)
	 * �¶��㣺(0,-1,0)��Ӧ(0,180)
	 *  ���������z�� ��(0,90)  z_p:2/90 = 0.0222f;
	 */
	//mvMat *= glm::translate(glm::mat4(1.0f), glm::vec3(0, -1, 0));//tMat (0,180��
	//mvMat *= glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 1));//tMat (0,90��
	mvMat *= glm::translate(glm::mat4(1.0f), glm::vec3(posit[0], posit[1], posit[2]));//tMat (0,90��
	//mvMat *= glm::rotate(glm::mat4(1.0f), 0.01f * (float)m_Observer.m_posMany.y(), glm::vec3(0, 1, 0));//rMat С����Ҫ��ת
	mvMat *= glm::scale(glm::mat4(1.0f), glm::vec3(0.05, 0.05, 0.05));//sMat

	//��͸�Ӿ����mv�����Ƹ���Ӧ��ͳһ����
	glUniformMatrix4fv(mvLocTwo, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLocTwo, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniform4f(costColorTwo, 1.0f, 0, 0.0f, 1.0f);

	//������� ͬ��������һ��
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	//���ö�������ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//�� С����
	glDrawElements(GL_TRIANGLES, X_SEGMENTS* Y_SEGMENTS * 6, GL_UNSIGNED_INT, 0);

	// ���VAO��VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

    _context.swapBuffer();
}