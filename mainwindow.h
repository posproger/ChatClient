#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "ccnetmanager.h"
#include "ccmanager.h"
#include "ccchannel.h"

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
    void connectedToServer(void);
    void disconnectedFromServer(void);
    void logined(void);
    void unlogined(void);
    void channelsReceived(QMap<int,CCChannel*>  map);
    void channelChosen(int index);

signals:
    void newMsgForSend(QString msg);
    void mkLogin(QString login, QString pwd, QString email);
    void mkConnect(void);

private slots:
    void reconnect(void);
    void login(void);
    void sendMsgBegin(void);

private:
    Ui::MainWindow *ui;
    CCManager m_manager;
    int m_curChannel;
    QMap<int,CCChannel*> m_channels;

};

#endif // MAINWINDOW_H
