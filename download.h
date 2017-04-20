#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <QObject>
#include <QTcpSocket>
#include <QDir>
#include <QFile>
#include "connection.h"
#include "strip.h"

class Download : public QObject
{
    Q_OBJECT
public:
    QString buff;
    QString *dPath;
    QString *url;
    QString *user;
    QString *pass;
    QString path;
    QString name;
    QWidget *wgt;

    explicit Download(QString *Url, QString *User, QString *Pass, QString Path, QString Name,QWidget *Wgt, QObject *parent = 0);
    ~Download();
    void getfile();

signals:
    void ending ();
    void error(int ,QString ,QString );
    void BytesReaded(int);
    void BytesToRead (int);
    void m_error(QString m_err);

public slots:
    void downloading();
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();
    void m_abort();

private:
    QFile *file;
    StrIp *strIp;
    Datawrite *dataw;
    QTcpSocket *socket;
    QTcpSocket *socketpasv;
    QString ip;
    quint16 port;
    QByteArray *arr;
    int bytesReadedInt;
};

#endif // DOWNLOAD_H
