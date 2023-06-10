#pragma once
#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H
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

constexpr int PIXWIDTH = 640; //480p
constexpr int PIXHIGHT = 480;
constexpr int FRAMERATE = 60;

//constexpr int PIXWIDTH = 1280; //720p
//constexpr int PIXHIGHT = 720;
//constexpr int FRAMERATE = 60;

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

#endif GLOBALCONFIG_H