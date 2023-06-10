#include "CVMat2QImage.h"
#include <QMessageLogContext>
#include <QDebug>
CVMat2QImage::CVMat2QImage()
{
}

CVMat2QImage::~CVMat2QImage()
{
}

/**
 * cv::Mat& 与cv::Mat之间性能相差2ms，前者为2ms后者为4ms
 */


QImage CVMat2QImage::QCVMat2QImage(const cv::Mat& mat)
{
    const unsigned char* _pMatData = mat.data;
    //delete _pMatData;
    //_pMatData = nullptr;
    //_pMatData = mat.data;

    int width = mat.cols;
    int height = mat.rows;
    int bytesPerLine = static_cast<int>(mat.step);
    switch (mat.type())
    {
        //8 bit , ARGB
    case CV_8UC4:
    {
        QImage image(_pMatData, width, height, bytesPerLine, QImage::Format_ARGB32);
        return image;
    }

    //8 bit BGR
    case CV_8UC3:
    {
        QImage image(_pMatData, width, height, bytesPerLine, QImage::Format_RGB888);
        //swap blue and red channel
        return image.rgbSwapped();
    }

    //8 bit Gray shale
    case CV_8UC1:
    {
        QImage image(_pMatData, width, height, bytesPerLine, QImage::Format_Grayscale8);
        return image;
    }

    //
    default:
    {
        //Unsupported format
        qWarning() << "Unsupported cv::Mat type:" << (qint8)mat.type()
            << ", Empty QImage will be returned!";
        return QImage();
    }
    }
}