#pragma once
#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H
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

constexpr int PIXWIDTH = 640; //480p
constexpr int PIXHIGHT = 480;
constexpr int FRAMERATE = 60;

//constexpr int PIXWIDTH = 1280; //720p
//constexpr int PIXHIGHT = 720;
//constexpr int FRAMERATE = 60;

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

#endif GLOBALCONFIG_H