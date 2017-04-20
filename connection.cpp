#include "connection.h"
#include <QString>
#include <QMessageBox>

Datawrite::Datawrite(QObject *parent) : QObject(parent)
{

}



//void Datawrite::readyRead()
//{



//    //        if (buff.contains("passive",Qt::CaseInsensitive))
//    //        {
//    //            ip.clear();
//    //            buff.toUtf8();

//    //            for (;!buff.startsWith("(");)
//    //                buff.remove(0,1);
//    //            buff.remove(0,1);

//    //            int n = buff.indexOf(")");
//    //            int s = buff.size();
//    //            for (int i=0; n<s;i++)
//    //            {
//    //                buff.chop(1);
//    //                s=buff.size();
//    //            }
//    //            qDebug()<<buff;
//    //            QString temp1;
//    //            QString temp2;
//    //            while (buff.indexOf(",")!=(-1))
//    //                buff.replace(buff.indexOf(","),1,".");
//    //            qDebug()<<"buff"<<buff;
//    //            int c = 0;
//    //            int i=0;
//    //            for (i=0;i<buff.size();)
//    //                if ((buff.indexOf(".")-i)==0)
//    //                {
//    //                    c++;
//    //                    if (c<4)
//    //                        ip+=buff.at(i);
//    //                    buff.remove(0,1);
//    //                }
//    //                else
//    //                    if(c<4)
//    //                    {
//    //                        ip+=buff.at(i);
//    //                        buff.remove(0,1);
//    //                    }
//    //                    else if(c==4)
//    //                    {
//    //                        temp1+=buff.at(i);
//    //                        buff.remove(0,1);

//    //                    }
//    //                    else if (c==5)
//    //                    {
//    //                        temp2+=buff.at(i);
//    //                        buff.remove(0,1);

//    //                    }
//    //            qDebug()<<"ip"<<ip;
//    //            qDebug()<<"temp1"<<temp1<<"temp2"<<temp2;


//    //            bool ok;
//    //            s = temp1.toInt(&ok);
//    //            if (!ok)
//    //                qDebug()<<"Error in temp1.ToInt";
//    //            n = temp2.toInt(&ok);
//    //            if(!ok)
//    //                qDebug()<<"error in temp2.ToInt";
//    //            port = s*256+n;

//    //            qDebug()<<port;

//    //        }
//    //    }
//    //    else
//    //    {
//    //        qDebug()<<"Socketpasv Reading!!!";
//    //        socketpasv->waitForReadyRead(0);
//    //        QFile file(QCoreApplication::applicationDirPath()+"/temp.txt");
//    //        file.open(QIODevice::Text | QIODevice::Append | QIODevice::WriteOnly);
//    //        QTextStream in (&file);
//    //        if (socketpasv->canReadLine())
//    //        {
//    //            in<<socketpasv->readAll();
//    //            qDebug()<<"Socketpasv has been read!!!!!!!";
//    //        }
//    //        else
//    //        {
//    //            qDebug()<<"closing";
//    //            file.close();
//    //            socket->close();
//    //            socketpasv->close();
//    //            return;
//    //        }
//    //        file.close();
//    //    }
//    //    if (!socket->waitForReadyRead(5000) & !socketpasv->waitForReadyRead(5000))
//    //        emit readed(QCoreApplication::applicationDirPath()+"/temp.txt");
//}

QByteArray Datawrite::writedata(QString str1, QString *str)
{
    while (str->contains("\r") | str->contains("\n") | str->contains("\r\n"))
    {
        if (str->contains("\n"))
            str->remove(str->indexOf("\n"),1);
        else if (str->contains("\r"))
            str->remove(str->indexOf("\r"),1);
        else if(str->contains("\r\n"))
        {
            str->remove(str->indexOf("\n"),1);
            str->remove(str->indexOf("\r"),1);

        }
    }
    *str+="\r\n";

    str1+=" "+*str;
    qDebug()<<str1;
    QByteArray snd(str1.toUtf8());
    return snd;

}

QByteArray Datawrite::writedata(QString str1)
{
    if (str1.contains("\n"))
        str1.remove(str1.indexOf("\n"),1);
    else if (str1.contains("\r"))
        str1.remove(str1.indexOf("\r"),1);
    else if(str1.contains("\r\n"))
    {
        str1.remove(str1.indexOf("\n"),1);
        str1.remove(str1.indexOf("\r"),1);

    }
    str1+="\r\n";

    qDebug()<<str1;
    QByteArray snd(str1.toUtf8());
    return snd;

}



