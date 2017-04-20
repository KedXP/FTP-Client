#ifndef GETLIST_H
#define GETLIST_H

#include <QObject>
#include <QTcpSocket>
#include <QTreeWidgetItem>
#include <connection.h>
#include "getlist.h"
#include "strip.h"

class GetList : public QObject
{
    Q_OBJECT
public:
    QString *url;
    QString *user;
    QString *pass;
    QString path;
    QTreeWidgetItem *itm;


    explicit GetList(QString *Url, QString *User, QString *Pass, QString Path, QTreeWidgetItem *Itm, QObject *parent = 0);
    void doItm(QTreeWidgetItem *);
    QString buff;
    ~GetList();


signals:
    void m_status (QString);
    void m_barval (int);
    void ending ();
    void error(int ,QString ,QString );

public slots:
    void list ();
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();


private:
    StrIp *strIp;
    Datawrite *dataw;
    QTcpSocket *socket;
    QTcpSocket *socketpasv;
    QString ip;
    quint16 port;
    QByteArray *arr;
    int m_msgSize;
    QTreeWidgetItem *itm1;
    bool check;
};

#endif // GETLIST_H
