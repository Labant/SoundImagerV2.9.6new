#pragma once
//#include <QMutex>
//#include "opencv2/core.hpp"
//#include <QQueue>

#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H
#include <QHostAddress>

//相机初始flag
//bool GLOBALCONFIG_CAMERAINITFLAG = false;
struct dataPack
{
	float* _p = nullptr;
	float* _x = nullptr;
	float* _y = nullptr;
	float* _z = nullptr;
};

//采集卡参数
constexpr int FS = 48000;
constexpr int SAMPLE = FS / 10;
constexpr int BITERSSAMPLE = 16;
constexpr int CHANNLENUMBER = 4;

//视频参数
//constexpr int PIXWIDTH = 848;//480p intel摄像头支持分辨率
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

//位于像素位置的机构体
#ifndef MYPOINT_STRUCT
#define MYPOINT_STRUCT
struct MyPoint
{
	int posX;
	int posY;
	int radius;
	int count;
};//Point 结构体包含了鼠标点击widget的具体位置，和绘制圆的半径以及被点击的次数，这个次数是整体权重计算的比例
#endif //! MYPOINT_STRUCT


//TCP link 参数
const QHostAddress PRESETMONITORIP("192.168.1.188");
constexpr quint16 PRESETMONITORPORT = 8234;

//Tcp  解析 参数
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