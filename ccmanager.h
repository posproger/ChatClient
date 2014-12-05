#ifndef CCMANAGER_H
#define CCMANAGER_H

#include <QObject>
#include <QMap>
#include "ccchannel.h"
#include "ccnetmanager.h"

class CCManager : public QObject
{
    Q_OBJECT
public:
    explicit CCManager(QObject *parent = 0);

signals:
    void connected(void);
    void disconnected(void);
    void msgReceived(QString msg);
    void msgForSend(QString msg);
    void make_connect(void);
    void make_disconnect(void);
    void logined(void);
    void unlogined(void);
    void channelsReceived(QMap<int,CCChannel*>);
    void persChannelsReceived(QMap<QString,CCChannel*>);

public slots:
    void mkConnect(void);
    void mkDisconnect(void);
    void netConnected(void);
    void netDisconnected(void);
    void mkLogin(QString login, QString pwd, QString email);
    void getChannels(void);
    void getPersChannels(void);
    void getChannelHistory(int channel);
    void newMsgReceived(QString msg);
    void newMsgForSend(QString msg, int channel);

private:
    QMap<int,CCChannel*> m_channels;
    QMap<QString,CCChannel*> m_persChannels;
    CCNetManager m_netManager;
    int m_curChannel;
    int m_loginStage;

};

#endif // CCMANAGER_H
