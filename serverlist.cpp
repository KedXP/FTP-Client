#include "serverlist.h"
#include "ui_serverlist.h"
#include "addserver.h"
#include <QLayout>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include <QList>
#include <QByteArray>


serverlist::serverlist(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::serverlist)
{
    ui->setupUi(this);
    ui->treeWidget->setColumnCount(4);
    QStringList headers;
    headers << tr("Server name") << tr("Server address")<<tr("Login")<<tr("Password");
    ui->treeWidget->setHeaderLabels(headers);
    ui->treeWidget->setRootIsDecorated(false);
    ui->treeWidget->setColumnHidden(2, true);
    ui->treeWidget->setColumnHidden(3, true);



    QObject::connect(this,SIGNAL(getlist()),this,SLOT(servchanged()));
    QObject::connect (ui->treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(on_pushButton_clicked()));
    //connect (ui->treeWidget, SIGNAL(itemActivated(QTreeWidgetItem*,int))


    emit getlist();


}

serverlist::~serverlist()
{
    delete ui;
}

void serverlist::on_cancel_clicked()              //Cancel
{
    close ();
}



void serverlist::on_pushButton_clicked()                //Connect
{
    int a = ui->treeWidget->currentColumn();
    if(a==(-1))
        QMessageBox::information(this,"Ups","Choose the server, please!");
    else
    {
        QTreeWidgetItem itmsend = *ui->treeWidget->currentItem();
        emit connect_cliced(itmsend);
        //close ();
    }

}

void serverlist::on_add_clicked()                         //Add server
{
    addserver *ns = new addserver(this);
    connect (ns, SIGNAL(listchanged()),this,SLOT (servchanged()));
    ns -> show();


}

void serverlist::on_edit_clicked()                        //Edit sever
{
    int a = ui->treeWidget->currentColumn();
    if(a!=(-1))
    {
        addserver *ns = new addserver(this);
        connect(ns, SIGNAL(listchanged()),this,SLOT(editego()));
        ns->show();
    }

}

void serverlist::servchanged()   //перечитывает файл с серверами
{
    ui->treeWidget->clear();
    QString pathdata = QCoreApplication::applicationDirPath() + "/data/"+"serverlist.txt";
    QFile serverlist2 (pathdata);
    serverlist2.open(QIODevice::Text | QIODevice::ReadOnly);
    while (!serverlist2.atEnd())
    {
        QString str1 = serverlist2.readLine(); // читаем первую строку
        QStringList str2 = str1.split(' ');// делим строку на кусочки... в качестве разделителя используем пробел
        QTreeWidgetItem *itm = new QTreeWidgetItem (4);

        for (int i=0;i<str2.size();i++) // перебираем все кусочки строки
            itm->setText(i,str2[i]); // переводим их int и записываем в новый массив

        ui->treeWidget->addTopLevelItem(itm);
    }

    serverlist2.close(); // закрываем файл
}

void serverlist::on_delete_2_clicked()  //DELETE SERVER
{
    int a = ui->treeWidget->currentColumn();
    if(a!=(-1))
    {
        QTreeWidgetItem *itm = new QTreeWidgetItem(4);
        itm = ui->treeWidget->currentItem();

        QString str;
        str = itm->text(0)+" "+itm->text(1)+" "+itm->text(2)+" "+itm->text(3);

        delserv123(&str, 0);

        emit getlist();
    }

}

void serverlist::editego()
{

    QTreeWidgetItem *itm = new QTreeWidgetItem(4);
    itm = ui->treeWidget->currentItem();

    QString str;
    str = itm->text(0)+" "+itm->text(1)+" "+itm->text(2)+" "+itm->text(3);

    delserv123(&str, 1);

    emit getlist();

}

void serverlist::delserv123 (QString *str, int check)
{


    QString pathdata = QCoreApplication::applicationDirPath() + "/data/";

    QFile serverlist2 (pathdata + "serverlist.txt");
    serverlist2.open(QIODevice::Text | QIODevice::ReadWrite);

    QFile serverlist3 (pathdata + "serverlist1.txt");
    serverlist3.open(QIODevice::Text | QIODevice::WriteOnly);
    QTextStream in (&serverlist3);

    int j=0;

    for (int i=0; !(serverlist2.atEnd());i++)
    {
        if (check==1)
        {
            QString t1emp = serverlist2.readLine(); // читаем строку
            if (!(t1emp == str))
            {
                in << t1emp;
            }
            else if (j==1)
            {in << t1emp;}
            else
            {j=1;}

        }
        else
            if (check==0)
            {
                QString t1emp = serverlist2.readLine(); // читаем строку
                if (!(t1emp == str))
                {
                    in << t1emp;
                }

            }

    }
    //конец бывшей функции
    serverlist2.close();
    serverlist3.close();
    if(serverlist2.remove())
        serverlist3.rename(pathdata+"serverlist.txt");

}


