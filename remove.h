#ifndef REMOVE_H
#define REMOVE_H

#include <QObject>
#include <QTcpSocket>
#include <QString>
#include "connection.h"



class m_Remove : public QObject
{
    Q_OBJECT
public:
    explicit m_Remove(QTcpSocket *Socket,QString *Path,QObject *parent);
    explicit m_Remove(QString *Url, QString *User, QString *Pass, QString *Path,QObject *parent);
    ~m_Remove();


signals:

    void m_status (QString);
    void error (int,QString,QString);
    void ending();

public slots:
    void m_rm();
    void readyread();
private:
    QString *url;
    QString *user;
    QString *pass;
    QString *path;
    Datawrite *m_write;
    QTcpSocket *socket;

};

#endif // REMOVE_H
