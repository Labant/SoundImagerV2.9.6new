#pragma once
#include <QObject>
#include <QString>

//2����ת10���� flagΪ���������;0Ϊ����1Ϊ��
static int binary2Int(QString& _binary, int _len, int flag = 0)
{

    //������������ֹλ������
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
        //ִ��15��
        for (int i = _binary.size() - 1; i > 1; i--)
        {
            QString _tem = (QString)_binary.at(i);

            _resNum += _tem.toInt() * pow(2, _prowNum);
            ++_prowNum;

            //qDebug() << "_resNum:" << _resNum;
        }
        break;
    case 1:
        //ִ��15��
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

//��
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
//��
static int ltZero(double _lt)
{
    _lt = -_lt;//תΪ����
    int _ltNum = 0;
    QString _binary;

    _binary.append('1');
    while (_ltNum < 15)
    {
        _lt = _lt * 2;
        //qDebug() << "lt:" << _lt;
        if (_lt < 1)
        {
            //ȡ��
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
    /**˼·��
    *       1���������������������ж��Ƿ�ȡ��
     *      2��*2������1���������1�������0��16�μ�16λ
     * ϸ�ڣ�����:����15λ�����λ(������𣬵�ַ������������)ǰ��0ռλ��
     *       ����������16λ�����λ��1
     */

    return dou > 0 ? gtZero(dou) : ltZero(dou);


}

class GenerateLvsFloat64ToInt
{


};

