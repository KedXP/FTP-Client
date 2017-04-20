#ifndef SERVERLIST_H
#define SERVERLIST_H

#include <QDialog>
#include <QString>
#include <QTreeWidgetItem>
#include <QFile>
//#include "addserver.h"

namespace Ui {
class serverlist;
}

class serverlist : public QDialog
{
    Q_OBJECT

public:
    explicit serverlist(QWidget *parent = 0);

    void delserv123 (QString *, int );

    ~serverlist();

private slots:
    void on_cancel_clicked();

    void on_pushButton_clicked();

    void on_add_clicked();

    void on_edit_clicked();

    void servchanged();

    void on_delete_2_clicked();

    void editego ();

signals:
    void getlist();
    void connect_cliced (QTreeWidgetItem itm);

private:
    Ui::serverlist *ui;
};

#endif // SERVERLIST_H
