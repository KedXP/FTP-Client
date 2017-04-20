#ifndef UPLOAD_H
#define UPLOAD_H

#include <QObject>
#include <QTcpSocket>
#include <QDir>
#include "connection.h"
#include "strip.h"

class Upload : public QObject
{
    Q_OBJECT
public:
    QString buff;
    QString *dPath;
    QString *url;
    QString *user;
    QString *pass;
    QString path;
    QString spath;
    QString Name;
    QByteArray bbuuff;

    explicit Upload(QString *Url, QString *User, QString *Pass, QString Path, QString sPath, QObject *parent = 0);
    ~Upload();



signals:
    void error(int, QString, QString);
    void b_toWrite(int);
    void b_Writed(int);
    void endingg ();
    void BytesReaded();
    void f_start();

public slots:
    void putfile();
    void aborted();
    void uploading();
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();

private:
    QFile file;
    StrIp *strIp;
    Datawrite *dataw;
    QTcpSocket *socket;
    QTcpSocket *socketpasv;
    QString ip;
    quint16 port;
    QByteArray *arr;
    int writed;
    int ch;
};

#endif // UPLOAD_H
