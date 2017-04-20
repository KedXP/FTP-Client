#include "download.h"
#include <QFile>
#include <QDataStream>
#include <QMessageBox>
#include <QDir>

Download::Download(QString *Url, QString *User, QString *Pass, QString Path, QString Name, QWidget *Wgt, QObject *parent) : QObject(parent)
{
    qDebug()<<"i'm in download";
    dPath = new QString;
    url=Url;
    user=User;
    pass=Pass;
    path=Path;
    name = Name;
    *dPath = QDir::homePath();
    *dPath+="/FtpDownload/";
    bytesReadedInt = 0;
    wgt = Wgt;

}

void Download::downloading()
{
    QDir dir;
    dir.mkdir(*dPath);
    buff.clear();
    buff= *dPath+name;
    qDebug()<<buff;
    file = new QFile(buff);
    if (!file->open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
            file->close();
            file->remove();
            emit error(0,"Can not create file!!!",file->errorString());
    }
    file->setTextModeEnabled(false);



    socket = new QTcpSocket(this);
    socketpasv = new QTcpSocket(this);

    connect (socket,SIGNAL(connected()),this,SLOT(connected()));
    connect (socket,SIGNAL(disconnected()),this,SLOT(disconnected()));
    connect (socket,SIGNAL(bytesWritten(qint64)),this,SLOT(bytesWritten(qint64)));
    connect (socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
    connect (socketpasv,SIGNAL(connected()),this,SLOT(connected()));
    connect (socketpasv,SIGNAL(disconnected()),this,SLOT(disconnected()));
    connect (socketpasv,SIGNAL(readyRead()),this,SLOT(readyRead()));

    qDebug()<<"Connecting: "<<*url;
    socket->connectToHost(*url,21);
    if (!socket->waitForConnected(30000))
        emit error(0,"Error connection: ","Can not connect to "+*url+":"+socket->errorString());

    socket->write(dataw->writedata("user",user));                        //socket->write("user ftp\r\n");
    //socket->waitForBytesWritten(2000);

    socket->write(dataw->writedata("pass",pass));
    //socket->waitForBytesWritten(2000);


    socket->write(dataw->writedata("pasv"));
    //socket->waitForReadyRead(3000);



}

void Download::readyRead()
{
    if (sender()==socket)
    {
        qDebug()<<"Reading...\n";
        buff.clear();
        buff=socket->readAll();
        qDebug()<<"Socket: "<<buff;
        if (buff.contains("530"))
        {
            emit error(0,"Authentification error","Can not connect: Login/Password incorrect!!!");

        }

        if (buff.contains("227"))
        {
            strIp->strToIP(buff,ip,port);
            qDebug()<<"Vivod"<<ip<<"   "<<port;
            socketpasv->connectToHost(ip,port);
            if (!socketpasv->waitForConnected(30000))
            {
                emit error(0,"Error connection","Can not connect: "+socketpasv->errorString());
            }
            socketpasv->setSocketOption(QAbstractSocket::KeepAliveOption,1);
            if (socketpasv->isTextModeEnabled())
            {
                socketpasv->setTextModeEnabled(false);
            }

            socket->write(dataw->writedata("retr",&path));
        }

        if (buff.contains("150"))
        {


            QString sstr;
            for (int i = buff.size()-1;i>-1;i--)
                if (buff.at(i)=='(')
                {
                    for (int j = i+1; buff.at(j)!=' ';j++)
                        sstr+=buff.at(j);
                    break;
                }
            emit BytesToRead(sstr.toInt());
            qDebug()<<"To read: "<<sstr;
        }

        if (buff.contains("226"))
        {
            emit ending();
            return;
        }

        if (buff.contains("550"))
        {
           emit error(0,"Error","Error downloading! Something went wrong!");
        }


    }
    if(sender()==socketpasv)
    {
        getfile();
    }
}

void Download::connected()
{
    qDebug()<<"Socket Connection OK!\n";
    //socket->waitForReadyRead(1000);

}


void Download::disconnected()
{
    if (sender()==socket)
        qDebug()<<"Socket Disconnected!";
    else if (sender() == socketpasv)
        qDebug()<<"Socketpasv Disconnected";
}

void Download::bytesWritten(qint64 bytes)
{
    qDebug()<<"Socket Wrote:"<<bytes;
    //socket->waitForReadyRead();
}

void Download::getfile()
{
    QByteArray bbuuff;
    bbuuff.clear();
    bbuuff=socketpasv->readAll();
    file->write(bbuuff);
    bytesReadedInt+=bbuuff.size();
    emit BytesReaded(bytesReadedInt);
}

void Download::m_abort()
{
    socketpasv->close();
    socket->close();
    file->close();
    file->remove();
    emit ending();

}

Download::~Download()
{
    file->close();
    //file->setFileName(name);
    qDebug()<<"destructor: killing Download!!!";
    socket->close();
    socketpasv->close();
    emit ending();
}


