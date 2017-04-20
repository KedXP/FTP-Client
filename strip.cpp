#include "strip.h"
#include <QDebug>

StrIp::StrIp(QObject *parent) : QObject(parent)
{

}

void StrIp::strToIP(QString &buff, QString &ip, quint16 &port)
{
    ip.clear();
    buff.toUtf8();

    for (;buff.indexOf("(")!=-1;)
        buff.remove(0,1);

    for (;buff.indexOf(")")!=-1;)
        buff.chop(1);

    qDebug()<<buff;
    QString temp1;
    QString temp2;
    while (buff.indexOf(",")!=(-1))
        buff.replace(buff.indexOf(","),1,".");
    qDebug()<<buff;
    int c = 0;
    for (int i=0;i<buff.size();)
        if ((buff.indexOf(".")-i)==0)
        {
            c++;
            if (c<4)
                ip+=buff.at(i);
            buff.remove(0,1);
        }
        else
            if(c<4)
            {
                ip+=buff.at(i);
                buff.remove(0,1);
            }
            else if(c==4)
            {
                temp1+=buff.at(i);
                buff.remove(0,1);

            }
            else if (c==5)
            {
                temp2+=buff.at(i);
                buff.remove(0,1);

            }
    qDebug()<<"ip"<<ip;
    qDebug()<<"temp1"<<temp1<<"temp2"<<temp2;


//    bool ok;
//    s = temp1.toInt(&ok);
//    if (!ok)
//        qDebug()<<"Error in temp1.ToInt";
//    n = temp2.toInt(&ok);
//    if(!ok)
//        qDebug()<<"error in temp2.ToInt";
    port = temp1.toInt()*256+temp2.toInt();

    qDebug()<<port;

    return;


}
