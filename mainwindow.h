#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QtGui>
#include <QProgressBar>
#include <QLabel>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:    
    QString *url;
    QString *user;
    QString *pass;
    QMenu *tmenu;
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

    void getcfpath (QString , QTreeWidgetItem *);    //получаем список файлов клиента

private slots:
  void on_actionConnect_To_triggered();

  void on_action_Addserver_triggered();

  void on_action_Disconnect_triggered();

  void on_actionExit_triggered();

  void on_action_From_Server_triggered();

  void on_action_To_Server_triggered();

  void on_action_My_Downloads_triggered();

  void on_action_About_triggered();

  void getlistclient(QString , QTreeWidgetItem *);

  void on_treeWidget_doubleClicked(const QModelIndex &index);

  void on_treeWidget_customContextMenuRequested(const QPoint &pos);

  void on_treeWidget_expanded(const QModelIndex &index);

  void connect11 (QTreeWidgetItem);

  void getlistserver (QModelIndex);

  void m_errror(int, QString, QString);

  void removing ();

  //void m_abort_npdial()


signals:
  void getlists(QString , QTreeWidgetItem *);

private:
  QProgressBar *m_pBar;
  QLabel *m_statusLabel;
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
