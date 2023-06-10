#pragma once
#include <QImage>
#include <opencv2/opencv.hpp>

using namespace cv;


class CVMat2QImage
{
public:
	CVMat2QImage();
	~CVMat2QImage();

public:
	static QImage QCVMat2QImage(const cv::Mat& mat);

private:

};

