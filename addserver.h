#ifndef ADDSERVER_H
#define ADDSERVER_H

#include <QDialog>

namespace Ui {
class addserver;
}

class addserver : public QDialog
{
    Q_OBJECT

public:
    explicit addserver(QWidget *parent = 0);
    ~addserver();

signals:
    void listchanged();

private slots:
    void on_Save_clicked();

    void on_Cancel_clicked();

private:
    Ui::addserver *ui;
};

#endif // ADDSERVER_H
