#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QAbstractSocket>
#include <QTreeWidgetItem>

class Datawrite : public QObject
{
    Q_OBJECT
public:
    explicit Datawrite(QObject *parent = 0);
    QByteArray writedata(QString str1, QString *);
    QByteArray writedata(QString str1);

private:
    QTcpSocket *socket;
    QTcpSocket *socketpasv;
    QByteArray *arr;
    int m_msgSize;
    QString buff;

signals:


public slots:

};

#endif // CONNECTION_H
