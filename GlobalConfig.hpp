#pragma once
//#include <QMutex>
//#include "opencv2/core.hpp"
//#include <QQueue>

#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H
#include <QHostAddress>

//�����ʼflag
//bool GLOBALCONFIG_CAMERAINITFLAG = false;
struct dataPack
{
	float* _p = nullptr;
	float* _x = nullptr;
	float* _y = nullptr;
	float* _z = nullptr;
};

//�ɼ�������
constexpr int FS = 48000;
constexpr int SAMPLE = FS / 10;
constexpr int BITERSSAMPLE = 16;
constexpr int CHANNLENUMBER = 4;

//��Ƶ����
//constexpr int PIXWIDTH = 848;//480p intel����ͷ֧�ֱַ���
//constexpr int PIXHIGHT = 480;
//constexpr int FRAMERATE = 60;

//constexpr int PIXWIDTH = 640; //480p
//constexpr int PIXHIGHT = 480;
//constexpr int FRAMERATE = 60;

//constexpr int PIXWIDTH = 1280; //720p
//constexpr int PIXHIGHT = 720;
//constexpr int FRAMERATE = 60;

//HK
constexpr int PIXWIDTH = 1920; //720p
constexpr int PIXHIGHT = 1080;
constexpr int FRAMERATE = 60;

//λ������λ�õĻ�����
#ifndef MYPOINT_STRUCT
#define MYPOINT_STRUCT
struct MyPoint
{
	int posX;
	int posY;
	int radius;
	int count;
};//Point �ṹ������������widget�ľ���λ�ã��ͻ���Բ�İ뾶�Լ�������Ĵ������������������Ȩ�ؼ���ı���
#endif //! MYPOINT_STRUCT


//TCP link ����
const QHostAddress PRESETMONITORIP("192.168.1.188");
constexpr quint16 PRESETMONITORPORT = 8234;

//Tcp  ���� ����
constexpr double GAIN = 0.688f;
constexpr double HEX800000 = 8388608.0f;
constexpr double HEX8000 = 32768.0f;
constexpr double NEED_16BITS = HEX8000 * GAIN;
constexpr double NEED_24BITS = HEX800000 * GAIN;
constexpr int PACKLENGTH = 0x04 * 256 + 4;
enum SAMPLEBITS
{
	BITS_16 = 0,
	BITS_24 = 1

};



#endif//! GLOBALCONFIG_H