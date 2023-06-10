#include "Test_openGL.h"
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QPainter>
#define PI 3.1415926

Test_openGL::Test_openGL(QWidget* parent) :m_angle(0), m_soundLocation(0.0,0.0)
{
	//ui->setupUi(this);

	//setMinimumSize(1000, 800);
	//旋转
	startTimer(2000 / 60);


	//向量 默认指向北极
	QVector3D _top{ 0,1,0 };
	float _step = 12;//遍历一遍纬度每一步的步长，越小越精细

	//VBO
	for (float _yaw = 0; _yaw <= 180; _yaw += _step)//遍历经度
	{
		_vertexMatrix << QVector<QVector3D>();
		m_col = 0;
		for (float _pitch = 0; _pitch < 360; _pitch += _step)//遍历纬度
		{
			QMatrix4x4 _mat;
			_mat.setToIdentity();//在一个单位内处理
			_mat.rotate(_yaw, 1, 0, 0);//X轴上，旋转一次步长
			_mat.rotate(-_pitch, 0, 1, 0);//Y轴上，旋转一次步长  -_pitch 

			auto _p = _top * _mat;//点乘
			_vertexMatrix[m_row] << _p;
			++m_col;//纬度循环值
		}
		++m_row;//经度循环值
	}

	//精度
	//m_presicionY = floor(longitude / m_row);//向下取整
	//m_presicionX = floor(latitude / m_col);
	m_presicionY = ceil(float(longitude / m_row));//向上取整  //8
	m_presicionX = ceil(float(latitude / m_col));			//8

	//VAO
	for (int y = 0; y < m_row - 1; ++y)//max 180 经度 m_row:23   180/23 = 7.83度
	{
		for (int x = 0; x < m_col; ++x)//max 360 纬度 m_col:45   360/45 = 8度
		{
			_p0 = _vertexMatrix[y][x];
			auto _p1 = _vertexMatrix[y + 1][x];

			int _nextX = x + 1;
			if (_nextX == m_col)//纬度重置为0，保证不越界
			{
				_nextX = 0;
			}
			auto _p2 = _vertexMatrix[y + 1][_nextX];
			auto _p3 = _vertexMatrix[y][_nextX];

			//每次输入的是一个矩形的位置
			m_vertices << Vertex{ { _p0.x(), _p0.y(), _p0.z() }, {(float)x / m_col,		(float)y / m_row} }
					   << Vertex{ { _p1.x(), _p1.y(), _p1.z() }, {(float)x / m_col,		(float)(y + 1) / m_row} }
					   << Vertex{ { _p2.x(), _p2.y(), _p2.z() }, {(float)(x + 1) / m_col,	(float)(y + 1) / m_row} }
					   << Vertex{ { _p3.x(), _p3.y(), _p3.z() }, {(float)(x + 1) / m_col,	(float)y / m_row} };
		}
	}



	m_camera.move(-6, 0, 3);
	m_camera.look(0, 30, 0);
	m_camera.update();

	////被子类监视
	////installEventFilter(&m_camera);
	//installEventFilter(&m_Observer);

}

Test_openGL::~Test_openGL()
{
	//delete ui;

	m_vao.deleteLater();
	m_program->deleteLater();
	m_program_point->deleteLater();

}

void Test_openGL::initializeGL()
{
	//初始化可以调用的opengl函数
	this->initializeOpenGLFunctions();
	//开启深度测试。近处可以挡住远处物体
	//glEnable(GL_DEPTH_TEST);
	//线框模式
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//设置背景色
	glClearColor(0, 0.5, 0.7, 1);
	//创建 vao、vbo对象,vao对应一个vbo
	m_vao.create();
	m_vbo.create();

	//图片
	//m_texture = new QOpenGLTexture(QImage("C:\\Users\\Administrator\\Desktop\\xx.jpg"));
	////m_texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
	////m_texture->setMagnificationFilter(QOpenGLTexture::Linear);

	//分配shader
	m_program = new QOpenGLShaderProgram();

	//分配shader 点
	m_program_point = new QOpenGLShaderProgram();

	//定点
	m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, u8R"(
	#version 330 core
	in vec3 vPos;
	in vec2 vTexture;
	out vec2 TexCoords;

	uniform mat4 projection;
	uniform mat4 view;
	uniform mat4 model;

	void main()
	{
		TexCoords = vTexture;

		gl_Position = projection * view * model * vec4(vPos, 1.0);
	}

)");
	//颜色
	m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, u8R"(
	#version 330 core
	out vec4 FragColor;
	in vec2 TexCoords;

	uniform vec3 lightColor;
	uniform sampler2D Texture;

	void main()
	{
		FragColor = vec4(texture(Texture, TexCoords).rgb * lightColor, 1.0);
		//FragColor = vec4(1.0,0.0,0.0, 1.0);
	} 

)");
	//应用shader
	m_program->link();

	//定点 点的
	m_program_point->addShaderFromSourceCode(QOpenGLShader::Vertex, u8R"(
	#version 330 core
	in vec3 vPos;
	in vec2 vTexture;
	out vec2 TexCoords;

	uniform mat4 projection;
	uniform mat4 view;
	uniform mat4 model;

	void main()
	{
		TexCoords = vTexture;

		gl_Position = projection * view * model * vec4(vPos, 1.0);
	}

)");

	//颜色 点的
	m_program_point->addShaderFromSourceCode(QOpenGLShader::Fragment, u8R"(
	#version 330 core
	out vec4 FragColor;
	in vec2 TexCoords;

	uniform vec3 lightColor;
	uniform sampler2D Texture;

	void main()
	{
		//FragColor = vec4(texture(Texture, TexCoords).rgb * lightColor, 1.0);
		FragColor = vec4(1.0,0.0,0.0, 1.0);
	} 

)");
	//应用shader
	m_program_point->link();

	//vertex array VAO
	// 顶点缓存中前三个是顶点坐标, 后两个是纹理坐标, 一个顶点由5个float值组成
	if (m_vertexCount < m_vertices.count())
	{
		if (m_vertexBuffer)
			delete[] m_vertexBuffer;
		m_vertexBuffer = new float[m_vertices.count() * 5];
		m_vertexCount = m_vertices.count();
		int _offset = 0;
		for (auto& vertex : m_vertices)
		{
			m_vertexBuffer[_offset] = vertex.pos.x(); _offset++;
			m_vertexBuffer[_offset] = vertex.pos.y(); _offset++;
			m_vertexBuffer[_offset] = vertex.pos.z(); _offset++;
			m_vertexBuffer[_offset] = vertex.texture.x(); _offset++;
			m_vertexBuffer[_offset] = vertex.texture.y(); _offset++;
		}
	}

	//vao vbo 绑定
	m_vao.bind();
	m_vbo.bind();

	//vbo绑定数据
	//m_vbo.allocate(_vertex, sizeof(_vertex));
	m_vbo.allocate(m_vertexBuffer, sizeof(float) * m_vertices.count() * 5);

	//shader 绑定
	m_program->bind();
	// 绑定顶点坐标信息, 从0 * sizeof(float)字节开始读取3个float, 因为一个顶点有5个float数据, 所以下一个数据需要偏移5 * sizeof(float)个字节
	m_program->setAttributeBuffer("vPos", GL_FLOAT, 0 * sizeof(float), 3, 5 * sizeof(float));
	m_program->enableAttributeArray("vPos");
	// 绑定纹理坐标信息, 从3 * sizeof(float)字节开始读取2个float, 因为一个顶点有5个float数据, 所以下一个数据需要偏移5 * sizeof(float)个字节
	m_program->setAttributeBuffer("vTexture", GL_FLOAT, 3 * sizeof(float), 2, 5 * sizeof(float));
	m_program->enableAttributeArray("vTexture");

	//view
	m_view.setToIdentity();
	//第一个参数表示你的眼睛在哪里，第二个参数表示你的眼睛看向了哪里，第三个参数表示你的头顶朝向是哪里;前两个向量是具体的坐标，第三个向量表示的是方向
	m_view.lookAt(QVector3D(3, 3, 3), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
	//m_view.lookAt(QVector3D(5, 0, 0), QVector3D(0, 0, 0), QVector3D(0, 1, 0));

	//model
	//在定时器中。增加旋转	

	m_program->release();
	m_vao.release();



}

void Test_openGL::resizeGL(int w, int h)
{
	//设置在一个单位矩阵上进行
	m_projection.setToIdentity();
	//设置视角投影 参数一：投影角度（宽）；参数二：横纵比;参数三、四：距离范围内的物体被显示
	m_projection.perspective(20, (float)w / h, 0.0001, 1000);

}

void Test_openGL::paintGL()
{
	//m_texture->bind();
	m_vao.bind();
	m_program->bind();
	// 绑定变换矩阵
	m_program->setUniformValue("projection", m_projection); //物体投影角度
	m_program->setUniformValue("view", m_camera.m_view);	//eye观察角度
	//m_program->setUniformValue("view", m_view);
	m_program->setUniformValue("model", m_Observer.m_model);			//物体旋转角度

	int _index = 0;
	 //绘制
	for (int i = 0; i < m_col * (m_row - 1); ++i)
	{
		//glPointSize(4);
		glDrawArrays(GL_TRIANGLE_FAN, _index, 4);//它允许我们逆时针定义四个顶点，然后绘制出来两个三角形组成一个矩形;_index：表示取值位置，4：表示取4个值
		_index += 4;
	}

	m_program->release();
	m_vao.release();
	//m_texture->release();

	//点
	m_program_point->bind();
	// 绑定变换矩阵
	m_program_point->setUniformValue("projection", m_projection); //投影角度
	m_program_point->setUniformValue("view", m_camera.m_view);	//观察角度
	//m_program_point->setUniformValue("view", m_view);
	m_program_point->setUniformValue("model", m_Observer.m_model);			//旋转角度
	
	//glPointSize(35);
	glPointSize(55);
	glColor3ub(255, 102, 102);
	glBegin(GL_POINTS);
	//qDebug() << "_p0:" << _p0;
	//m_soundLocation[x:360][y:180]
	sherePos(m_soundLocation.y(), m_soundLocation.x());

	//qDebug() << "m_sherePos_X:" << m_sherePos_X <<" "<< "m_sherePos_Y:" << m_sherePos_Y;
	
	//顶点的位置
	if (m_sherePos_Y == 0 &&  m_sherePos_X == 0)
	{
		//qDebug() << "1!";
		qDebug() << "_vertexMatrix.at(m_sherePos_Y).at(m_sherePos_X).x():" << _vertexMatrix.at(m_sherePos_Y).at(m_sherePos_X).x() << " " << "_vertexMatrix.at(m_sherePos_Y).at(m_sherePos_X).y():" << _vertexMatrix.at(m_sherePos_Y).at(m_sherePos_X).y();
		glVertex3f(_vertexMatrix.at(m_sherePos_Y).at(m_sherePos_X).x(), _vertexMatrix.at(m_sherePos_Y).at(m_sherePos_X).y(), _vertexMatrix.at(m_sherePos_Y).at(m_sherePos_X).z());
	}
	else if(m_sherePos_Y != 0 && m_sherePos_X != 0)
	{
		//qDebug() << "2!";
		//0.706773   -0.669131   -0.229644
		qDebug() << _vertexMatrix.at(m_sherePos_Y - 1).at(m_sherePos_X - 1).x() <<" "<<_vertexMatrix.at(m_sherePos_Y - 1).at(m_sherePos_X - 1).y()<<" "<< _vertexMatrix.at(m_sherePos_Y - 1).at(m_sherePos_X - 1).z();
		glVertex3f(_vertexMatrix.at(m_sherePos_Y - 1).at(m_sherePos_X - 1).x(), _vertexMatrix.at(m_sherePos_Y - 1).at(m_sherePos_X - 1).y(), _vertexMatrix.at(m_sherePos_Y - 1).at(m_sherePos_X - 1).z());
	}

	glEnd();
	m_program_point->release();
}

void Test_openGL::timerEvent(QTimerEvent* event)
{
	m_camera.update();

	m_angle += 1;
	if (m_angle >= 360)
		m_angle = 0;
	m_model.setToIdentity();
	//m_model.rotate(m_angle, 0, 1, 0);//m_angle
	m_model.rotate(m_angle, 1, 0, 0);//m_angle
	//m_model.translate(0, 0, 1);
	repaint();


}
//鼠标按压后进行相关操作
void Test_openGL::enterEvent(QEvent* event)
{

}

void Test_openGL::leaveEvent(QEvent* event)
{

}


void Test_openGL::mousePressEvent(QMouseEvent* event)
{
	m_camera.m_bool = true;

	m_posStart = event->pos();
	//qDebug() << "m_posStart:" << m_posStart;

}

void Test_openGL::mouseReleaseEvent(QMouseEvent* event)
{
	m_camera.m_bool = false;
}

void Test_openGL::mouseMoveEvent(QMouseEvent* event)
{

	if (!m_camera.m_bool)
		return;

	//移动的过程
	m_pomoving = event->pos();
	//qDebug() << "m_pomoving:" << m_pomoving;
	//移动的多少
	m_camera.m_posMany = m_pomoving - m_posStart;
	m_Observer.m_posMany = m_pomoving - m_posStart;
	//开始的位置移到最新起点
	m_posStart = m_pomoving;

	//m_posMany = QPoint(event->pos().x() - m_posStart.x(), event->pos().y() - m_posStart.y());



	//qDebug() << QString::fromLocal8Bit("移动了多少：")<< m_posMany;
}

void Test_openGL::sherePos(int y, int x)
{
	if (!m_presicionX)
	{
		m_sherePos_X = 0;
	}
	else
	{
		m_sherePos_X = x / m_presicionX;
	}
	if (!m_presicionY)
	{
		m_sherePos_Y = 0;
	}
	else
	{
		m_sherePos_Y = y / m_presicionY;
		//qDebug() << "m_sherePos_Y:" << m_sherePos_Y;
		//弥补小球y轴（上下反）
		m_sherePos_Y = 15 - m_sherePos_Y;
		//qDebug() << "y:" << y;
		//qDebug() << "m_presicionY:" << m_presicionY;
		//qDebug() << "m_sherePos_Y:" << m_sherePos_Y;
	}
	
}
