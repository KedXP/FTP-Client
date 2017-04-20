#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "serverlist.h"
#include "addserver.h"
#include <QDir>
#include <QStringList>
#include <QTreeWidget>
#include <QFileInfo>
#include <QDebug>
#include <QtGui>
#include <QMessageBox>
#include <QModelIndex>
#include <QThread>
#include <QInputDialog>
#include <QFileDialog>
#include <QProgressDialog>
#include <QAction>
#include "connection.h"
#include "getlist.h"
#include "download.h"
#include "upload.h"
#include "remove.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->splitter);
    QObject::connect(this,SIGNAL(getlists(QString,QTreeWidgetItem*)),this ,SLOT(getlistclient(QString,QTreeWidgetItem*)));
    QObject::connect (ui->treeWidget_2,SIGNAL(expanded(QModelIndex)),this, SLOT(getlistserver(QModelIndex)));
    connect (ui->treeWidget_2, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(on_treeWidget_customContextMenuRequested(QPoint)));

    m_pBar = new QProgressBar;
    m_statusLabel = new QLabel;
    m_statusLabel->setBuddy(m_pBar);
    m_pBar->setFixedWidth(60);
    m_statusLabel->setFocusPolicy(Qt::NoFocus);
    m_statusLabel->setStyleSheet("border: 0px solid green;border-radius: 0px;padding: 0px");
    ui->statusBar->addWidget(m_pBar);
    ui->statusBar->addWidget(m_statusLabel);
    ui->statusBar->setFixedHeight(20);
    m_pBar->setMaximum(1);
    m_pBar->setMinimum(0);

    m_statusLabel->setText("Disconnected");

    url = new QString;
    pass = new QString;
    user = new QString;

    ui->treeWidget->clear();
    ui->treeWidget_2->clear();
    QTreeWidgetItem *citm = new QTreeWidgetItem (2);
    QTreeWidgetItem *sitm = new QTreeWidgetItem (2);

    ui->treeWidget->setColumnCount(2);
    ui->treeWidget_2->setColumnCount(2);
    //   ui->treeWidget->setColumnHidden(1,true);
    //   ui->treeWidget_2->setColumnHidden(1,true);

    QStringList list;
    list<<"Your Files"<<"File Path";
    ui->treeWidget->setHeaderLabels(list);
    list.clear();
    list<<"Your server files"<<"FilePath";
    ui->treeWidget_2->setHeaderLabels(list);
    list.clear();


    QString chpath = QDir::homePath(); //определяем домашнюю папку пользователя на компьютере(ользователя)
    citm->setText(0,chpath);
    citm->setText(1,chpath);
    citm->setIcon(0,QPixmap(":/Img/Img/folder.png"));
    ui->treeWidget->addTopLevelItem(citm);
    //delete citm;
    sitm->setText(0,"Server");
    sitm->setIcon(0,QPixmap(":/Img/Img/folder.png"));
    sitm->setText(1,"");
    sitm->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
    ui->treeWidget_2->addTopLevelItem(sitm);

    ui->treeWidget->setColumnHidden(1,true);
    ui->treeWidget_2->setColumnHidden(1,true);

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_action_Addserver_triggered()          //MainWindow menubar Add server
{
    addserver *Add= new addserver(this);
    Add -> show();
}


void MainWindow::on_actionConnect_To_triggered()              //MainWindow menubar Connect to
{


    serverlist *servlist = new serverlist(this);
    QDialog::connect(servlist,SIGNAL(connect_cliced(QTreeWidgetItem)),this,SLOT(connect11(QTreeWidgetItem)));

    servlist -> show();


}

void MainWindow::on_action_Disconnect_triggered()       //MainWindow menubar Disconnect
{
    ui->treeWidget_2->setCurrentItem(ui->treeWidget_2->topLevelItem(0));
    for(;ui->treeWidget_2->currentItem()->childCount()>0;)
        ui->treeWidget_2->currentItem()->removeChild(ui->treeWidget_2->currentItem()->child(0));
}



void MainWindow::on_actionExit_triggered()            //MainWindow menubar Exit
{
    close ();
}

void MainWindow::on_action_From_Server_triggered()    //MainWindow menubar Download from
{
    if (url->isEmpty() | url->isNull())
    {
        QMessageBox *msg = new QMessageBox(this);
        if (msg->warning(this,"Error!","Connect to the server please!",QMessageBox::Ok))
        {
            on_actionConnect_To_triggered();
        }
        msg->deleteLater();
        return;
    }

    QProgressDialog *m_pdial = new QProgressDialog();
    QPushButton *cancButt;
    m_pdial->setWindowFlags(Qt::Dialog | Qt::WindowMinimizeButtonHint);
    m_pdial->setParent(0);
    m_pdial->setWindowTitle("Downloading...");
    m_pdial->setCancelButton(cancButt);
    m_pdial->setCancelButtonText("&Abort");
    m_pdial->setMinimum(0);
    m_pdial->setMaximum(0);


    QThread *thrd2 = new QThread (this);
    QString Path123;
    QString Name123;
    if (ui->treeWidget_2->currentColumn()==-1)
    {
        bool ok;
        Path123= QInputDialog::getText(this, tr("Set Server's File Path"),
                                       tr("File Path:"), QLineEdit::Normal,
                                       "", &ok);
        if (ok)
        {
            Name123 = Path123;
            while (Name123.count("/")>0)
                Name123.remove(0,1);
        }
    }
    if (ui->treeWidget_2->currentColumn()!=-1)
    {
        Path123 = ui->treeWidget_2->currentItem()->text(1);
        Name123 = ui->treeWidget_2->currentItem()->text(0);
    }

    m_pdial->setLabelText("Downloading: "+Name123);




    Download *dwn = new Download(url,user,pass,Path123,Name123,this,0);

    connect(dwn,SIGNAL(BytesToRead(int)),m_pdial,SLOT(setMaximum(int)));
    connect (dwn,SIGNAL(BytesReaded(int)),m_pdial,SLOT(setValue(int)));

    connect (dwn,SIGNAL(error(int ,QString ,QString )),this,SLOT(m_errror(int,QString,QString)));
    connect (dwn,SIGNAL(ending()),m_pdial,SLOT(deleteLater()));
    connect (m_pdial,SIGNAL(canceled()),dwn,SLOT(m_abort()));
    connect (dwn,SIGNAL(ending()),thrd2,SLOT(quit()));
    connect (thrd2,SIGNAL(finished()),dwn,SLOT(deleteLater()));
    connect (thrd2,SIGNAL(started()),dwn,SLOT(downloading()));
    connect (thrd2,SIGNAL(finished()),thrd2,SLOT(deleteLater()));
    connect (dwn, SIGNAL(ending()),dwn,SLOT(deleteLater()));

    m_pdial->show();

    dwn->moveToThread(thrd2);
    thrd2->start();
}

void MainWindow::on_action_To_Server_triggered()     //MainWindow menubar Download to
{

    if (url->isEmpty() | url->isNull())
    {
        QMessageBox *msg = new QMessageBox(this);
        if (msg->warning(this,"Error!","Connect to the server please!",QMessageBox::Ok))
        {
            on_actionConnect_To_triggered();
        }
        msg->deleteLater();
        return;
    }

    QThread *thrd3 = new QThread (this);
    QString Path1234;
    QList<QString> Name1234;

    if (ui->treeWidget_2->currentColumn()!=-1 || ui->treeWidget->currentColumn()!=-1 )
    {
        Path1234 = ui->treeWidget->currentItem()->text(1);

        QProgressDialog *m_pdial1 = new QProgressDialog();
        QPushButton *cancButt;
        m_pdial1->setWindowFlags(Qt::Dialog | Qt::WindowMinimizeButtonHint);
        m_pdial1->setParent(0);
        m_pdial1->setWindowTitle("Downloading...");
        m_pdial1->setCancelButton(cancButt);
        m_pdial1->setCancelButtonText("&Abort");
        m_pdial1->setMinimum(0);
        m_pdial1->setMaximum(0);

        Upload *upL = new Upload(url,user,pass,Path1234,ui->treeWidget_2->currentItem()->text(1),0);

        connect(upL,SIGNAL(b_toWrite(int)),m_pdial1,SLOT(setMaximum(int)));
        connect (upL,SIGNAL(b_Writed(int)),m_pdial1,SLOT(setValue(int)));


        connect (upL,SIGNAL(endingg()),thrd3,SLOT(quit()));
        connect (thrd3,SIGNAL(finished()),upL,SLOT(deleteLater()));
        connect (thrd3,SIGNAL(started()),upL,SLOT(uploading()));
        connect (thrd3,SIGNAL(finished()),thrd3,SLOT(deleteLater()));
        connect (upL, SIGNAL(endingg()),upL,SLOT(deleteLater()));
        connect (upL,SIGNAL(error(int ,QString ,QString )),this,SLOT(m_errror(int,QString,QString)));
        connect (m_pdial1,SIGNAL(canceled()),upL,SLOT(aborted()));
        //connect (m_pdial1,SIGNAL(canceled()),this,SLOT(removing()));
        connect (upL,SIGNAL(endingg()),m_pdial1,SLOT(deleteLater()));

        m_pdial1->show();
        m_pdial1->setLabelText("Uploading: "+Path1234);
        upL->moveToThread(thrd3);
        thrd3->start();

    }
}


void MainWindow::on_action_My_Downloads_triggered()    //MainWindow menubar My downloads
{

}

void MainWindow::on_action_About_triggered()     //MainWindow menubar About
{

}

void MainWindow::connect11(QTreeWidgetItem itm)      //Connecting to server
{
    url = new QString(itm.text(1));
    user = new QString(itm.text(2));
    pass=new QString(itm.text(3));
    delete sender();

    m_pBar->setEnabled(true);
    m_pBar->setMaximum(0);

    for (;ui->treeWidget_2->topLevelItem(0)->childCount()>0;)
        ui->treeWidget_2->topLevelItem(0)->removeChild(ui->treeWidget_2->topLevelItem(0)->child(0));

    if (ui->treeWidget_2->currentColumn()==-1)
    {
        if(!ui->treeWidget_2->topLevelItem(0)->isExpanded())
        {
            ui->treeWidget_2->topLevelItem(0)->setExpanded(true);
        }
    }
    else     if (!ui->treeWidget_2->currentItem()->isExpanded())
        ui->treeWidget_2->currentItem()->setExpanded(true);

    QThread *thrd = new QThread (this);
    GetList *lst = new GetList(url,user,pass,ui->treeWidget_2->topLevelItem(0)->text(1),ui->treeWidget_2->topLevelItem(0));

    connect (lst,SIGNAL(m_status(QString)),m_statusLabel,SLOT(setText(QString)));
    connect (lst,SIGNAL(ending()),thrd,SLOT(quit()));
    connect (thrd,SIGNAL(finished()),lst,SLOT(deleteLater()));
    connect (thrd,SIGNAL(started()),lst,SLOT(list()));
    connect (thrd,SIGNAL(finished()),thrd,SLOT(deleteLater()));
    connect (lst, SIGNAL(ending()),lst,SLOT(deleteLater()));
    connect (lst,SIGNAL(m_barval(int)),m_pBar,SLOT(setMaximum(int)));
    connect (lst,SIGNAL(m_barval(int)),m_pBar,SLOT(setValue(int)));
    connect (lst,SIGNAL(error(int ,QString ,QString )),this,SLOT(m_errror(int,QString,QString)));

    lst->moveToThread(thrd);
    thrd->start();



}

void MainWindow::removing()
{
    QString *m_fullpath = new QString;
    *m_fullpath= ui->treeWidget_2->currentItem()->text(1);
    qDebug()<<*m_fullpath;
    m_Remove *rm123 = new m_Remove(url,user,pass,m_fullpath,0);
    QThread *trd_rm = new QThread(this);

    connect(trd_rm,SIGNAL(started()),rm123,SLOT(m_rm()));
    connect (rm123,SIGNAL(ending()),rm123,SLOT(deleteLater()));
    connect (rm123,SIGNAL(ending()),trd_rm,SLOT(quit()));
    connect (trd_rm,SIGNAL(finished()),rm123,SLOT(deleteLater()));
    connect (trd_rm,SIGNAL(finished()),trd_rm,SLOT(deleteLater()));

    rm123->moveToThread(trd_rm);
    trd_rm->start();

}

void MainWindow::getlistclient(QString chpath, QTreeWidgetItem *itm)        //getting system's treewidget
{
    getcfpath(chpath,itm);
}

void MainWindow::getcfpath(QString path, QTreeWidgetItem *itm)      //function
{
    QDir folder(path);
    int check = 0;
    QList<QFileInfo> list= folder.entryInfoList();
    for(int i = 0; i < list.size(); i++)
    {
        if (check>=0 && !list[i].isDir())
        {
            QTreeWidgetItem *itm1 = new QTreeWidgetItem(2);
            itm1->setText(0,list[i].fileName());
            itm1->setText(1,list[i].absoluteFilePath());
            itm1->setIcon(0,QPixmap(":/Img/Img/file.png"));
            itm->addChild(itm1);

            //if (list[i].isDir() && list[i].fileName()!="." && list[i].fileName()!="..")
            //getcfpath(list[i].absoluteFilePath(),itm1);
        }
        else
            if (list[i].fileName()!="." && list[i].fileName()!="..")
            {
                QTreeWidgetItem *itm1 = new QTreeWidgetItem(2);
                //QTreeWidgetItem *itm2 = new QTreeWidgetItem(2);

                itm1->setIcon(0,QPixmap(":/Img/Img/folder.png"));
                //itm2->setIcon(0,QPixmap(":/Img/Img/folder.png"));
                itm1->setText(0,list[i].fileName());
                itm1->setText(1,list[i].absoluteFilePath());
                itm1->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
                itm->addChild(itm1);
            }


        check++;
    }
}



void MainWindow::on_treeWidget_doubleClicked(const QModelIndex &index)      //Expanding on double_click
{
    if (!(ui->treeWidget->currentItem()->isExpanded()))
    {
        int a = ui->treeWidget->currentItem()->childCount();
        for (int i=0; i<=(a);i++)
            ui->treeWidget->currentItem()->removeChild(ui->treeWidget->currentItem()->child(0));
        emit getlists (ui->treeWidget->currentItem()->text(1) ,ui->treeWidget->currentItem());
    }
}

void MainWindow::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
    qDebug ()<<"QMENU";
    if (sender()==ui->treeWidget)
    {
        QMenu *m_Menu = new QMenu(ui->treeWidget);
        m_Menu->addAction(ui->action_To_Server);

        m_Menu->move(ui->treeWidget->mapToGlobal(pos));
        m_Menu->show();
    }
    else
    {
        QMenu *m_Menu = new QMenu(ui->treeWidget_2);
        m_Menu->addAction(ui->action_From_Server);
        QAction *m_delete = new QAction("Delete",this);
        m_Menu->addAction(m_delete);
        connect (m_delete,SIGNAL(triggered(bool)),this,SLOT(removing()));
        m_Menu->exec(ui->treeWidget_2->mapToGlobal(pos));
    }
}



void MainWindow::on_treeWidget_expanded(const QModelIndex &index)       //expanding by expanding click
{
    ui->treeWidget->setCurrentIndex(index);
    for (int i=0; ui->treeWidget->currentItem()->childCount()>0;)
        ui->treeWidget->currentItem()->removeChild(ui->treeWidget->currentItem()->child(0));
    emit getlists (ui->treeWidget->currentItem()->text(1) ,ui->treeWidget->currentItem());
    ui->treeWidget->currentItem()->setExpanded(true);
}

void MainWindow::getlistserver(QModelIndex ind)
{
    ui->treeWidget_2->setCurrentIndex(ind);
    if (ui->treeWidget_2->currentItem()->childIndicatorPolicy()==0)
    {
        if (url->isEmpty() | url->isNull())
        {
            on_actionConnect_To_triggered();
        }
        else
        {
            for (;ui->treeWidget_2->currentItem()->childCount()>0;)
                ui->treeWidget_2->currentItem()->removeChild(ui->treeWidget_2->currentItem()->child(0));

            QThread *thrd1 = new QThread (this);
            GetList *lst = new GetList(url,user,pass,ui->treeWidget_2->currentItem()->text(1),ui->treeWidget_2->currentItem());

            m_pBar->setMaximum(0);
            connect (lst,SIGNAL(m_status(QString)),m_statusLabel,SLOT(setText(QString)));
            connect (lst,SIGNAL(m_barval(int)),m_pBar,SLOT(setMaximum(int)));
            connect (lst,SIGNAL(m_barval(int)),m_pBar,SLOT(setValue(int)));

            connect (lst,SIGNAL(ending()),thrd1,SLOT(quit()));
            connect (thrd1,SIGNAL(finished()),lst,SLOT(deleteLater()));
            connect (thrd1,SIGNAL(started()),lst,SLOT(list()));
            connect (thrd1,SIGNAL(finished()),thrd1,SLOT(deleteLater()));
            connect (lst, SIGNAL(ending()),lst,SLOT(deleteLater()));


            lst->moveToThread(thrd1);
            thrd1->start();
        }
    }
}

void MainWindow::m_errror(int a,QString title1,QString text1)
{
    QMessageBox *msg = new QMessageBox(this);
    switch (a)
    {
    case 0:
        msg->information(this,title1,text1);
        //    case 1:
        //        msg->warning(this,title1,text1);
    }

}

