#include "Test_openGL.h"
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QPainter>
#define PI 3.1415926

Test_openGL::Test_openGL(QWidget* parent) :m_angle(0), m_soundLocation(0.0,0.0)
{
	//ui->setupUi(this);

	//setMinimumSize(1000, 800);
	//��ת
	startTimer(2000 / 60);


	//���� Ĭ��ָ�򱱼�
	QVector3D _top{ 0,1,0 };
	float _step = 12;//����һ��γ��ÿһ���Ĳ�����ԽСԽ��ϸ

	//VBO
	for (float _yaw = 0; _yaw <= 180; _yaw += _step)//��������
	{
		_vertexMatrix << QVector<QVector3D>();
		m_col = 0;
		for (float _pitch = 0; _pitch < 360; _pitch += _step)//����γ��
		{
			QMatrix4x4 _mat;
			_mat.setToIdentity();//��һ����λ�ڴ���
			_mat.rotate(_yaw, 1, 0, 0);//X���ϣ���תһ�β���
			_mat.rotate(-_pitch, 0, 1, 0);//Y���ϣ���תһ�β���  -_pitch 

			auto _p = _top * _mat;//���
			_vertexMatrix[m_row] << _p;
			++m_col;//γ��ѭ��ֵ
		}
		++m_row;//����ѭ��ֵ
	}

	//����
	//m_presicionY = floor(longitude / m_row);//����ȡ��
	//m_presicionX = floor(latitude / m_col);
	m_presicionY = ceil(float(longitude / m_row));//����ȡ��  //8
	m_presicionX = ceil(float(latitude / m_col));			//8

	//VAO
	for (int y = 0; y < m_row - 1; ++y)//max 180 ���� m_row:23   180/23 = 7.83��
	{
		for (int x = 0; x < m_col; ++x)//max 360 γ�� m_col:45   360/45 = 8��
		{
			_p0 = _vertexMatrix[y][x];
			auto _p1 = _vertexMatrix[y + 1][x];

			int _nextX = x + 1;
			if (_nextX == m_col)//γ������Ϊ0����֤��Խ��
			{
				_nextX = 0;
			}
			auto _p2 = _vertexMatrix[y + 1][_nextX];
			auto _p3 = _vertexMatrix[y][_nextX];

			//ÿ���������һ�����ε�λ��
			m_vertices << Vertex{ { _p0.x(), _p0.y(), _p0.z() }, {(float)x / m_col,		(float)y / m_row} }
					   << Vertex{ { _p1.x(), _p1.y(), _p1.z() }, {(float)x / m_col,		(float)(y + 1) / m_row} }
					   << Vertex{ { _p2.x(), _p2.y(), _p2.z() }, {(float)(x + 1) / m_col,	(float)(y + 1) / m_row} }
					   << Vertex{ { _p3.x(), _p3.y(), _p3.z() }, {(float)(x + 1) / m_col,	(float)y / m_row} };
		}
	}



	m_camera.move(-6, 0, 3);
	m_camera.look(0, 30, 0);
	m_camera.update();

	////���������
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
	//��ʼ�����Ե��õ�opengl����
	this->initializeOpenGLFunctions();
	//������Ȳ��ԡ��������Ե�סԶ������
	//glEnable(GL_DEPTH_TEST);
	//�߿�ģʽ
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//���ñ���ɫ
	glClearColor(0, 0.5, 0.7, 1);
	//���� vao��vbo����,vao��Ӧһ��vbo
	m_vao.create();
	m_vbo.create();

	//ͼƬ
	//m_texture = new QOpenGLTexture(QImage("C:\\Users\\Administrator\\Desktop\\xx.jpg"));
	////m_texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
	////m_texture->setMagnificationFilter(QOpenGLTexture::Linear);

	//����shader
	m_program = new QOpenGLShaderProgram();

	//����shader ��
	m_program_point = new QOpenGLShaderProgram();

	//����
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
	//��ɫ
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
	//Ӧ��shader
	m_program->link();

	//���� ���
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

	//��ɫ ���
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
	//Ӧ��shader
	m_program_point->link();

	//vertex array VAO
	// ���㻺����ǰ�����Ƕ�������, ����������������, һ��������5��floatֵ���
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

	//vao vbo ��
	m_vao.bind();
	m_vbo.bind();

	//vbo������
	//m_vbo.allocate(_vertex, sizeof(_vertex));
	m_vbo.allocate(m_vertexBuffer, sizeof(float) * m_vertices.count() * 5);

	//shader ��
	m_program->bind();
	// �󶨶���������Ϣ, ��0 * sizeof(float)�ֽڿ�ʼ��ȡ3��float, ��Ϊһ��������5��float����, ������һ��������Ҫƫ��5 * sizeof(float)���ֽ�
	m_program->setAttributeBuffer("vPos", GL_FLOAT, 0 * sizeof(float), 3, 5 * sizeof(float));
	m_program->enableAttributeArray("vPos");
	// ������������Ϣ, ��3 * sizeof(float)�ֽڿ�ʼ��ȡ2��float, ��Ϊһ��������5��float����, ������һ��������Ҫƫ��5 * sizeof(float)���ֽ�
	m_program->setAttributeBuffer("vTexture", GL_FLOAT, 3 * sizeof(float), 2, 5 * sizeof(float));
	m_program->enableAttributeArray("vTexture");

	//view
	m_view.setToIdentity();
	//��һ��������ʾ����۾�������ڶ���������ʾ����۾����������������������ʾ���ͷ������������;ǰ���������Ǿ�������꣬������������ʾ���Ƿ���
	m_view.lookAt(QVector3D(3, 3, 3), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
	//m_view.lookAt(QVector3D(5, 0, 0), QVector3D(0, 0, 0), QVector3D(0, 1, 0));

	//model
	//�ڶ�ʱ���С�������ת	

	m_program->release();
	m_vao.release();



}

void Test_openGL::resizeGL(int w, int h)
{
	//������һ����λ�����Ͻ���
	m_projection.setToIdentity();
	//�����ӽ�ͶӰ ����һ��ͶӰ�Ƕȣ����������������ݱ�;���������ģ����뷶Χ�ڵ����屻��ʾ
	m_projection.perspective(20, (float)w / h, 0.0001, 1000);

}

void Test_openGL::paintGL()
{
	//m_texture->bind();
	m_vao.bind();
	m_program->bind();
	// �󶨱任����
	m_program->setUniformValue("projection", m_projection); //����ͶӰ�Ƕ�
	m_program->setUniformValue("view", m_camera.m_view);	//eye�۲�Ƕ�
	//m_program->setUniformValue("view", m_view);
	m_program->setUniformValue("model", m_Observer.m_model);			//������ת�Ƕ�

	int _index = 0;
	 //����
	for (int i = 0; i < m_col * (m_row - 1); ++i)
	{
		//glPointSize(4);
		glDrawArrays(GL_TRIANGLE_FAN, _index, 4);//������������ʱ�붨���ĸ����㣬Ȼ����Ƴ����������������һ������;_index����ʾȡֵλ�ã�4����ʾȡ4��ֵ
		_index += 4;
	}

	m_program->release();
	m_vao.release();
	//m_texture->release();

	//��
	m_program_point->bind();
	// �󶨱任����
	m_program_point->setUniformValue("projection", m_projection); //ͶӰ�Ƕ�
	m_program_point->setUniformValue("view", m_camera.m_view);	//�۲�Ƕ�
	//m_program_point->setUniformValue("view", m_view);
	m_program_point->setUniformValue("model", m_Observer.m_model);			//��ת�Ƕ�
	
	//glPointSize(35);
	glPointSize(55);
	glColor3ub(255, 102, 102);
	glBegin(GL_POINTS);
	//qDebug() << "_p0:" << _p0;
	//m_soundLocation[x:360][y:180]
	sherePos(m_soundLocation.y(), m_soundLocation.x());

	//qDebug() << "m_sherePos_X:" << m_sherePos_X <<" "<< "m_sherePos_Y:" << m_sherePos_Y;
	
	//�����λ��
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
//��갴ѹ�������ز���
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

	//�ƶ��Ĺ���
	m_pomoving = event->pos();
	//qDebug() << "m_pomoving:" << m_pomoving;
	//�ƶ��Ķ���
	m_camera.m_posMany = m_pomoving - m_posStart;
	m_Observer.m_posMany = m_pomoving - m_posStart;
	//��ʼ��λ���Ƶ��������
	m_posStart = m_pomoving;

	//m_posMany = QPoint(event->pos().x() - m_posStart.x(), event->pos().y() - m_posStart.y());



	//qDebug() << QString::fromLocal8Bit("�ƶ��˶��٣�")<< m_posMany;
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
		//�ֲ�С��y�ᣨ���·���
		m_sherePos_Y = 15 - m_sherePos_Y;
		//qDebug() << "y:" << y;
		//qDebug() << "m_presicionY:" << m_presicionY;
		//qDebug() << "m_sherePos_Y:" << m_sherePos_Y;
	}
	
}
