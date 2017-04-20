#include "upload.h"
#include <QFile>
#include <QDataStream>
#include <QMessageBox>
#include <QDir>
#include <QProgressDialog>
#include <QCoreApplication>
#include "remove.h"

Upload::Upload(QString *Url, QString *User, QString *Pass, QString Path, QString sPath, QObject *parent) : QObject(parent)
{
    qDebug()<<"i'm in Upload";
    dPath = new QString;
    url=Url;
    user=User;
    pass=Pass;
    path=Path;
    spath = sPath;
    Name.clear();
    for (int i = Path.size()-1; Path.at(i)!='/';i--)
        Name.insert(0,Path.at(i));


    *dPath = QDir::homePath();
    *dPath+="/FtpDownload/";
    writed = 0;
    ch=0;
}

void Upload::uploading()
{
    file.setFileName(path);
    if (file.exists())
    {
        if (!file.open(QIODevice::WriteOnly | QIODevice::ReadOnly))
        {
            emit error(0,"Error","Reading File: "+file.errorString());
            file.close();
            file.remove();
            return;
        }
    }
    else
    {
        emit error(0,"Error","File "+path+" doesn't exists!");
        return;
    }

    socket = new QTcpSocket(this);
    socketpasv = new QTcpSocket(this);

    connect (socket,SIGNAL(connected()),this,SLOT(connected()));
    connect (socket,SIGNAL(disconnected()),this,SLOT(disconnected()));
    connect (socket,SIGNAL(bytesWritten(qint64)),this,SLOT(bytesWritten(qint64)));
    connect (socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
    connect (socketpasv,SIGNAL(connected()),this,SLOT(connected()));
    connect (socketpasv,SIGNAL(disconnected()),this,SLOT(disconnected()));
    connect (socketpasv,SIGNAL(readyRead()),this,SLOT(readyRead()));
    connect (socketpasv,SIGNAL(bytesWritten(qint64)),this,SLOT(bytesWritten(qint64)));
    connect (this,SIGNAL(f_start()),this,SLOT(putfile()));

    qDebug()<<"Connecting: "<<*url;
    socket->connectToHost(*url,21);
    if (!socket->waitForConnected(3000))
    {
        emit error(0,"Error connection: ","Can not connect to "+*url+":"+socket->errorString());
        return;
    }

    socket->write(dataw->writedata("user",user));                        //socket->write("user ftp\r\n");
    socket->waitForBytesWritten(2000);


    socket->write(dataw->writedata("pass",pass));
    socket->waitForBytesWritten(2000);


    socket->write(dataw->writedata("pasv"));
    socket->waitForReadyRead(3000);


    socketpasv->connectToHost(ip,port);
    if (!socketpasv->waitForConnected(3000))
    {
        emit error(0,"Error connection","connecting to pasv: "+socketpasv->errorString());
        return;
    }

    socketpasv->setSocketOption(QAbstractSocket::KeepAliveOption,1);
    if (socketpasv->isTextModeEnabled())
    {
        socketpasv->setTextModeEnabled(false);
    }

    while ((spath.endsWith("/")) | (spath.size()==0))
        spath.chop(1);

    QString sstr = spath+"/"+Name;
    socket->write(dataw->writedata("stor",&sstr));

    socket->waitForReadyRead();

    //socket->waitForReadyRead();
    //socketpasv->waitForConnected();


        putfile();


}

void Upload::readyRead()
{
    if (sender()==socket)
    {
        buff.clear();
        buff=socket->readAll();
        qDebug()<<"Socket: "<<buff;
        if (buff.contains("530 Login incorrect"))
        {
            emit error(0,"Authentification error","Can not connect: Login/Password incorrect!!!");
            return;

        }

        if (buff.contains("227 Entering Passive",Qt::CaseInsensitive))
        {
            strIp->strToIP(buff,ip,port);
            qDebug()<<"Vivod"<<ip<<"   "<<port;
        }

        if (buff.contains("550"))
            emit error (0,"Error","Error writing file! Check access rights.");

        if (buff.contains("150"))
        {
            emit b_toWrite(file.size());
            //putfile();
            //emit f_start();
            //            bbuuff.clear();
            //            qDebug()<<"i ewe";
            //            bbuuff= file.read(10);
            //            writed+=bbuuff.size();
            //            emit b_Writed(writed);
            //            socketpasv->write(bbuuff);
            //            bytesWritten(10);

        }
        if (buff.contains("553"))
        {
            emit error (0,"Error","Error writing file! Check access rights.");
            aborted();
        }


    }

}

void Upload::connected()
{
    qDebug()<<"Socket Connection OK!\n";
    socket->waitForReadyRead(1000);

}


void Upload::disconnected()
{
    if (sender()==socket)
        qDebug()<<"Socket Disconnected!";
    else if (sender() == socketpasv)
        qDebug()<<"Socketpasv Disconnected";
}

void Upload::bytesWritten(qint64 bytes)
{
    if (sender()==socket)
    {
        //void putfile();
        qDebug()<<"Socket Wrote:"<<bytes;
        socket->waitForReadyRead();
    }
    else
        writed+=bytes;
    emit b_Writed(writed);

}

void Upload::putfile()
{
    qDebug()<<"Mi zawli";


    buff.clear();
    file.setTextModeEnabled(false);

    if (17000*8<(file.size()-writed))
    {
        bbuuff= file.read(17000*8);
        //writed+=bbuuff.size();
        //emit b_Writed(writed);
        socketpasv->write(bbuuff);
        socketpasv->waitForBytesWritten();
        emit f_start();
        return;
    }
    else if ((file.size()-writed)>=0)
    {
        bbuuff= file.read(file.size()-writed);
        writed+=bbuuff.size();
        emit b_Writed(writed);
        socketpasv->write(bbuuff);
        file.close();
        emit endingg();
        socketpasv->close();
        socket->close();
        deleteLater();
    }
}

void Upload::aborted()
{
    qDebug()<<"destructor: killing Upload!!!";

    socketpasv->close();
    //disconnect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
    socket->write(dataw->writedata("ABOR"));
    //socket->waitForReadyRead();
    QString wri;
    //    wri=socket->readAll();
    //    wri.clear();
    wri = spath+"/"+Name;
    socket->write(dataw->writedata("DELE",&wri));
    //    //socket->waitForReadyRead();
    //    wri=socket->readAll();
    wri.clear();
    socket->close();
    file.close();
    emit endingg();
    deleteLater();
}

Upload::~Upload()
{
    qDebug()<<"destructor: killing Upload!!!";
    if (!file.atEnd())
    {
        socket->write("ABOR");
        socket->waitForReadyRead();
        QString wri = spath+"/"+Name;
        socket->write(dataw->writedata("DELE",&wri));
        socket->waitForReadyRead();
    }
    socket->close();
    socketpasv->close();
    file.close();
    emit endingg();

}
