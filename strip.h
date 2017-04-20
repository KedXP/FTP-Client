#ifndef STRIP_H
#define STRIP_H

#include <QObject>

class StrIp : public QObject
{
    Q_OBJECT
public:
    explicit StrIp(QObject *parent = 0);
    void strToIP (QString &,QString &, quint16 &);

signals:

public slots:
};

#endif // STRIP_H
