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

public slots:
    void mkConnect(void);
    void mkDisconnect(void);
    void netConnected(void);
    void netDisconnected(void);
    void mkLogin(QString login);
    void newMsgReceived(QString msg);
    void newMsgForSend(QString msg);

private:
    QMap<int,CCChannel*> m_channels;
    CCNetManager m_netManager;
    int curChannel;

};

#endif // CCMANAGER_H
