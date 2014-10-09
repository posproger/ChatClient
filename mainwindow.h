#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

//class CCNetManager;
#include "ccnetmanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void newMsgCome(QString msg);
    void sendMsgBegin();
    void connectedToServer();
    void disconnectedFromServer();
    void reconnect();

signals:
    void newMsgForSend(QString msg);

private slots:
    void on_pbSend_clicked();

private:
    Ui::MainWindow *ui;
    CCNetManager m_netmanager;

};

#endif // MAINWINDOW_H
