#include "QFile"
#include <QStringList>
#include <QString>
#include <QTextStream>
#include <QApplication>
#include <QTreeWidgetItem>
#include "connection.h"
#include "getlist.h"
#include "strip.h"

GetList::GetList(QString *Url, QString *User, QString *Pass, QString Path, QTreeWidgetItem * Itm, QObject *parent) :
    QObject(parent)
{
    url=Url;
    user=User;
    pass=Pass;
    path=Path;
    itm=Itm;
    check = false;
}

void GetList::list()
{

    QFile file(QCoreApplication::applicationDirPath()+"/temp.txt");
    file.open(QIODevice::Text | QIODevice::Truncate | QIODevice::WriteOnly);
    file.close();

    socket = new QTcpSocket(this);
    socketpasv = new QTcpSocket(this);

    connect (socket,SIGNAL(connected()),this,SLOT(connected()));
    connect (socket,SIGNAL(disconnected()),this,SLOT(disconnected()));
    connect (socket,SIGNAL(bytesWritten(qint64)),this,SLOT(bytesWritten(qint64)));
    connect (socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
    connect (socketpasv,SIGNAL(connected()),this,SLOT(connected()));
    connect (socketpasv,SIGNAL(disconnected()),this,SLOT(disconnected()));
    connect (socketpasv,SIGNAL(readyRead()),this,SLOT(readyRead()));

    emit m_status("Connecting: "+*url);
    socket->connectToHost(*url,21);
    if (!socket->waitForConnected(3000))
    {
        emit m_status("Disconnected");
        emit error(1,"Connection error!",("Error connecting to "+*url));
        emit ending();
    }

    emit m_status("Authentification");

    socket->write(dataw->writedata("user",user));                        //socket->write("user ftp\r\n");
    //   socket->waitForBytesWritten(2000);


    socket->write(dataw->writedata("pass",pass));
    //   socket->waitForBytesWritten(2000);


    socket->write(dataw->writedata("pasv"));
    //   socket->waitForReadyRead(3000);




}

void GetList::readyRead()
{
    if (sender()==socket)
    {
        buff.clear();
        buff=socket->readAll();
        qDebug()<<buff;
        if (buff.startsWith("530")|buff.contains("\r\n530")|buff.contains("\n530"))
        {
            emit m_status("Disconnected!");
            emit error(1,"Authentification error!","Login or password incorrect");
            emit ending();

        }

        if (buff.startsWith("227")|buff.contains("\r\n227")|buff.contains("\n227"))
        {
            emit ("Connection to passive");
            strIp->strToIP(buff,ip,port);
            qDebug()<<"Vivod"<<ip<<"   "<<port;

            socketpasv->connectToHost(ip,port);
            if (!socketpasv->waitForConnected(30000))
            {
                emit m_status("Disconnected");
                emit error(0,"Troubles with server","Connection was lost! Try again later!");
                emit ending();
            }

            emit m_status("Downloading Files list!");
            socket->write(dataw->writedata("list",&path));

        }
        if (buff.startsWith("550")|buff.contains("\r\n550")|buff.contains("\n550"))
            check = true;

    }
    if(sender()==socketpasv)
    {


        QFile file(QCoreApplication::applicationDirPath()+"/temp.txt");
        if (!file.open(QIODevice::Text | QIODevice::Append | QIODevice::WriteOnly))
        {
            emit m_status("Disconnected");
            emit error (1,"Error with temp file","I can't create temp file! Something wrong with your acces!");
            emit ending();
        }
        QTextStream in (&file);
        while (socketpasv->bytesAvailable()>0)
        {

            in<<socketpasv->readLine();
            socketpasv->waitForReadyRead();
        }

        file.close();
        doItm(itm);
        emit m_status("Connected!");
        emit ending();


    }
}

void GetList::doItm(QTreeWidgetItem *itm)
{
    QFile file(QCoreApplication::applicationDirPath()+"/temp.txt");
    if(file.open(QIODevice::Text | QIODevice::ReadOnly))
    {
        while (!file.atEnd())
        {
            QTreeWidgetItem *itm1= new QTreeWidgetItem(2);
            buff=file.readLine();
            if ((buff.startsWith("d")))
            {
                itm1->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
                QStringList lst = buff.split(' ');

                while (lst.contains(""))
                {
                    lst.removeOne("");
                }

                QString ttmp;
                ttmp.clear();
                ttmp = lst[lst.size()-1];
                while (ttmp.indexOf("\n")!=-1)
                {
                    if (ttmp.contains("\r\n"))
                        ttmp.replace(ttmp.indexOf("\r\n"),4,"");
                    if (ttmp.indexOf("\n")!=-1)
                        ttmp.replace(ttmp.indexOf("\n"),2,"");
                }
                //qDebug()<<lst;
                itm1->setText(0,ttmp);
                ttmp.clear();
                ttmp=itm->text(1)+itm1->text(0)+"/";
                itm1->setText(1,ttmp);
                itm1->setIcon(0,QPixmap(":/Img/Img/folder.png"));
                itm->addChild(itm1);
            }
            else if (buff.startsWith("-"))
            {
                itm1->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicator);
                QStringList lst = buff.split(' ');

                while (lst.contains(""))
                {
                    lst.removeOne("");
                }

                QString ttmp;
                ttmp.clear();
                ttmp = lst[lst.size()-1];
                while (ttmp.indexOf("\n")!=-1)
                {
                    if (ttmp.contains("\r\n"))
                        ttmp.replace(ttmp.indexOf("\r\n"),2,"");
                    if (ttmp.indexOf("\n")!=-1)
                        ttmp.replace(ttmp.indexOf("\n"),2,"");
                }
                //qDebug()<<lst;
                itm1->setText(0,ttmp);
                ttmp.clear();
                ttmp=itm->text(1)+itm1->text(0);
                itm1->setText(1,ttmp);
                itm1->setIcon(0,QPixmap(":/Img/Img/file.png"));
                itm->addChild(itm1);

            }
            else if (buff.startsWith("l"))
            {
                QStringList lst = buff.split(' ');
                QString ttmp;
                ttmp.clear();

                while (lst.contains(""))
                {
                    lst.removeOne("");
                }

                for (int i=0;i<lst.size();i++)
                {

                    ttmp=lst[i];
                    while (ttmp.indexOf("\n")!=-1)
                    {
                        if (ttmp.contains("\r\n"))
                            ttmp.replace(ttmp.indexOf("\r\n"),4,"");
                        if (ttmp.indexOf("\n")!=-1)
                            ttmp.replace(ttmp.indexOf("\n"),2,"");
                    }
                    lst[i]=ttmp;
                }
                qDebug()<<lst;
                itm1->setText(0,lst[lst.size()-3]);
                ttmp.clear();
                ttmp=itm->text(1)+itm1->text(0);
                socket->write(dataw->writedata("size",&ttmp));
                //socket->waitForBytesWritten();
                socket->waitForReadyRead();
                if (check)
                {
                    ttmp.clear();
                    ttmp=itm->text(1)+itm1->text(0)+"/";
                    itm1->setText(1,ttmp);
                    itm1->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
                    itm1->setIcon(0,QPixmap(":/Img/Img/folder.png"));
                    itm->addChild(itm1);
                }
                else
                {
                    itm1->setText(1,ttmp);
                    itm1->setIcon(0,QPixmap(":/Img/Img/file.png"));
                    itm1->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicator);
                    itm->addChild(itm1);

                }
                check = false;

            }

        }

    }
    else
    {
        emit m_status ("Disconnected");
        emit error(1,"Error with temp file","It seems me, that you have no access rights for working with temp files =)");
        emit ending();
    }
}

void GetList::connected()
{
    qDebug()<<"Socket Connection OK!\n";
}


void GetList::disconnected()
{
    if (sender()==socket)
        qDebug()<<"Socket Disconnected!";
    else if (sender() == socketpasv)
        qDebug()<<"Socketpasv Disconnected";
}

void GetList::bytesWritten(qint64 bytes)
{
    qDebug()<<"Socket Wrote:"<<bytes;
    //socket->waitForReadyRead();
}

GetList::~GetList()
{
    QFile file(QCoreApplication::applicationDirPath()+"/temp.txt");
    file.remove();
    qDebug()<<"destructor: killing GetList!!!";
    socket->close();
    socketpasv->close();
    emit m_barval(1);
    emit ending();
}
