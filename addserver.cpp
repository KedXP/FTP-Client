#include "addserver.h"
#include "ui_addserver.h"
#include <QFile>
#include <QApplication>
#include <QTextStream>
#include <QIODevice>
#include <QDebug>
#include <QDir>
addserver::addserver(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addserver)
{
    ui->setupUi(this);
    ui->groupBox->show();
    ui->groupBox_2->show();
}

addserver::~addserver()
{
    delete ui;
}

void addserver::on_Save_clicked()
{
    QDir *dir = new QDir;
    dir->mkdir(QCoreApplication::applicationDirPath()+"/data/");
    QFile file (QCoreApplication::applicationDirPath() + "/data/"+"serverlist.txt");
    if (file.open(QIODevice::Text | QIODevice::WriteOnly | QIODevice::Append))
    {
        QTextStream in(&file);
        qDebug()<<ui->lAddr->text();
        in<<ui->lName->text()<<" "<<ui->lAddr->text()<<" "<<ui->lLog->text()<<" "<<ui->lPass->text()<<"\n";
        file.close();
        emit listchanged();

    }
    else
        qDebug()<<"file wasn't open!!!";
    close();

}

void addserver::on_Cancel_clicked()
{
    close();
}

