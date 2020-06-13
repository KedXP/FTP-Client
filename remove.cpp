#include "remove.h"

m_Remove::m_Remove(QTcpSocket *Socket,QString *Path,QObject *parent) : QObject(parent)
{
        socket=Socket;
        path=Path;
}

m_Remove::m_Remove(QString *Url, QString *User, QString *Pass, QString *Path,QObject *parent) : QObject(parent)
{
        url=Url;
        user=User;
        pass=Pass;
        path=Path;
}
//Just a commment
void m_Remove::m_rm()
{

    //connect (socket,SIGNAL(readyRead()),this,SLOT(readyread()));
    if (url->isEmpty() | url->isNull())
    {
        socket->write(m_write->writedata("ABOR"));
        socket->write(m_write->writedata("DELE",path));
        emit ending();
        return;
    }
    else
    {
        socket = new QTcpSocket(this);
        connect (socket,SIGNAL(readyRead()),this,SLOT(readyread()));

        emit m_status("Connecting: "+*url);
        socket->connectToHost(*url,21);
        if (!socket->waitForConnected(3000))
        {
            emit m_status("Disconnected");
            emit error(1,"Connection error!","Error connecting to "+*url);
            emit ending();
        }

        emit m_status("Authentification");

        socket->write(m_write->writedata("user",user));
        socket->waitForReadyRead();


        socket->write(m_write->writedata("pass",pass));
        socket->waitForReadyRead();

        QString *ssstr = new QString();\
        *ssstr = "Deleting file"+*path;
        emit m_status (*ssstr);

        //socket->write((m_write->writedata("ABOR")));

        socket->write(m_write->writedata("DELE",path));
        emit ending();


    }
}


void m_Remove::readyread()
{
    QString buff;
    buff.clear();
    buff=socket->readAll();
    qDebug()<<buff;
    if (buff.startsWith("530")|buff.contains("\r\n530")|buff.contains("\n530"))
    {
        emit m_status("Disconnected!");
        emit error(1,"Authentification error!","Login or password incorrect");
        socket->close();
        emit ending();

    }

    if (buff.startsWith("250")|buff.contains("\r\n250")|buff.contains("\n250"))
    {
        emit m_status ("Connected!");
        socket->close();
        emit ending();
    }
    if (buff.startsWith("550")|buff.contains("\r\n550")|buff.contains("\n550"))
    {
        emit m_status("Connected!");
        QString ssstr;
        ssstr = "Can not delete file"+*path;
        emit error(1,"Error",ssstr);
        socket->close();
        emit ending();

    }

}
m_Remove::~m_Remove()
{
    socket->close();
    deleteLater();

}
