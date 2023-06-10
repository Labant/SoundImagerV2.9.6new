#pragma once
#include <QObject>
#include <QString>

//2进制转10进制 flag为正负数标记;0为正，1为负
static int binary2Int(QString& _binary, int _len, int flag = 0)
{

    //对于正数，防止位数不齐
    if (_len % 4 != 0)
    {
        for (int i = _len % 4; i > 0; i--)
        {
            _binary.append('0');
        }
    }

    int _resNum = 0;
    double _prowNum = 0;

    switch (flag)
    {
    case 0:
        //执行15次
        for (int i = _binary.size() - 1; i > 1; i--)
        {
            QString _tem = (QString)_binary.at(i);

            _resNum += _tem.toInt() * pow(2, _prowNum);
            ++_prowNum;

            //qDebug() << "_resNum:" << _resNum;
        }
        break;
    case 1:
        //执行15次
        for (int i = _binary.size() - 1; i >= 0; i--)
        {
            QString _tem = (QString)_binary.at(i);

            //qDebug() << "_tem.toInt() * pow(2, _prowNum):" << _tem.toInt() * pow(2, _prowNum);
            _resNum += _tem.toInt() * pow(2, _prowNum);
            ++_prowNum;
            //qDebug() << "_resNum:" << _resNum;
        }
        break;
    }
    return _resNum;
}

//正
static int gtZero(double gt)
{
    int _gtNum = 0;
    QString _binary;

    _binary.append('0');
    while (_gtNum < 15)
    {
        gt = gt * 2;
        //qDebug() << "gt:" << gt;
        if (gt < 1)
        {
            _binary.append('0');
        }
        else
        {
            _binary.append('1');
            gt -= 1;
        }
        ++_gtNum;
    }
    return binary2Int(_binary, _binary.size());
}
//负
static int ltZero(double _lt)
{
    _lt = -_lt;//转为正数
    int _ltNum = 0;
    QString _binary;

    _binary.append('1');
    while (_ltNum < 15)
    {
        _lt = _lt * 2;
        //qDebug() << "lt:" << _lt;
        if (_lt < 1)
        {
            //取反
            //_binary.append('0');
            _binary.append('1');
        }
        else
        {
            //_binary.append('1');
            _binary.append('0');
            _lt -= 1;
        }
        ++_ltNum;
    }
    //qDebug() << "_binary:" << _binary;
    return binary2Int(_binary, _binary.size(), 1);
}


static int double2Int(double dou)
{
    /**思路：
    *       1、首先区分正负，用于判断是否取反
     *      2、*2。大于1，数组添加1否则添加0，16次即16位
     * 细节：正数:计算15位，最高位(最左边起，地址从右往左增大)前添0占位符
     *       负数：计算16位，最高位置1
     */

    return dou > 0 ? gtZero(dou) : ltZero(dou);


}

class GenerateLvsFloat64ToInt
{


};

