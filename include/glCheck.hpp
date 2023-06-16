#pragma once
#include <GL/glew.h>
#include <malloc.h>
#include <QDebug>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

/***
* ʹ�÷����ڡ������ͼ��ѧ��̡�һ�� --P15
*/
namespace glCheck
{


	//��GLSL�������ʧ��ʱ����ʾOpenGL��־����
	static void printShaderLog(GLuint shader)
	{
		int len = 0;
		int chWrittn = 0;
		char* log;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
		if (len > 0)
		{
			log = (char*)malloc(len);
			glGetShaderInfoLog(shader, len, &chWrittn, log);
			qDebug() << "Shader Info Log:" << log;
			free(log);
		}
	}

	//��GLSL����ʧ��ʱ����ʾOpenGL��־���ݡ�
	static void printProgramLog(int prog)
	{
		int len = 0;
		int chWrittn = 0;
		char* log;
		glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
		if (len > 0)
		{
			log = (char*)malloc(len);
			glGetProgramInfoLog(prog, len, &chWrittn, log);
			qDebug() << "Shader Info Log:" << log;
			free(log);
		}
	}

	//���OpenGL�����־�����Ƿ���OpenGl����
	static bool checkOpenGLError()
	{
		bool foundError = false;
		int glErr = glGetError();
		while (glErr != GL_NO_ERROR)
		{
			qDebug() << "glError:" << glErr;
			foundError = true;
			glErr = glGetError();
		}
		return foundError;
	}
	//��ȡglsl�ļ�
	static string readShaderSource(const char* filePath)
	{
		string content;
		ifstream fileStream(filePath, ios::in);
		string line = "";
		for (; !fileStream.eof();)
		{
			getline(fileStream, line);

			//if (fileStream.eof()) { content.append(line); break; }
			content.append(line + "\n");
		}
		fileStream.close();
		return content;
	}
	//������ɫ��
	static GLuint createShaderProgram(string vFilePath, string fFilePath)
	{
		qDebug() << checkOpenGLError();

		string vertShaderStr = readShaderSource(vFilePath.c_str());
		string fragShaderStr = readShaderSource(fFilePath.c_str());

		const char* vertShaderSrc = vertShaderStr.c_str();
		const char* fragShaderSrc = fragShaderStr.c_str();

		qDebug() << checkOpenGLError();
		GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
		qDebug() << checkOpenGLError();
		GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
		qDebug() << checkOpenGLError();
		glShaderSource(vShader, 1, &vertShaderSrc, NULL);
		qDebug() << checkOpenGLError();
		glShaderSource(fShader, 1, &fragShaderSrc, NULL);
		qDebug() << checkOpenGLError();
		glCompileShader(vShader);
		//check
		GLint vertCompiled;

		glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
		if (vertCompiled != 1) {
			qDebug() << "vertex compilation failed";
			printShaderLog(vShader);
		}

		glCompileShader(fShader);
		//check
		qDebug() << checkOpenGLError();
		glGetShaderiv(fShader, GL_COMPILE_STATUS, &vertCompiled);
		if (vertCompiled != 1) {
			qDebug() << "vertex compilation failed";
			printShaderLog(fShader);
		}

		GLuint vfProgram = glCreateProgram();

		glAttachShader(vfProgram, vShader);
		glAttachShader(vfProgram, fShader);
		glLinkProgram(vfProgram);
		//check
		qDebug() << checkOpenGLError();
		glGetShaderiv(vfProgram, GL_COMPILE_STATUS, &vertCompiled);
		if (vertCompiled != 1) {
			qDebug() << "vertex compilation failed";
			printShaderLog(vfProgram);
		}

		return vfProgram;
	}
}