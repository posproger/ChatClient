#ifndef CCNETMANAGER_H
#define CCNETMANAGER_H

#include <QObject>
#include <QtWebSockets/QtWebSockets>

class QUrl;

class CCNetManager : public QObject
{
    Q_OBJECT
public:
    explicit CCNetManager(QObject *parent = 0);

    bool isConnected(void) { return m_isConnected; }

signals:
    void connected(void);
    void disconnected(void);
    void msgReceived(QString msg);

public slots:
    void make_connect(void);
    void make_disconnect(void);
    void ws_connected(void);
    void ws_disconnected(void);
    void ws_error(QAbstractSocket::SocketError error);
    void newMsgForSend(QString msg);

private:
    QWebSocket *m_socket;
    bool m_isConnected;
    QUrl m_url;

};

#endif // CCNETMANAGER_H
