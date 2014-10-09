#include "ccnetmanager.h"

#include <QDebug>
#include <QAbstractSocket>

CCNetManager::CCNetManager(QObject *parent) :
    QObject(parent)
{
    m_socket = new QWebSocket();
    m_isConnected = false;
    m_url.setScheme("ws");
    m_url.setHost("localhost");
    m_url.setPort(8082);
    connect(m_socket,SIGNAL(connected()),this,SLOT(ws_connected()));
    connect(m_socket,SIGNAL(disconnected()),this,SLOT(ws_disconnected()));
    connect(m_socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(ws_error(QAbstractSocket::SocketError)));
    connect(m_socket,SIGNAL(textMessageReceived(QString)),this,SIGNAL(msgReceived(QString)));
}

void CCNetManager::make_connect(void) {
    qDebug() << "make_connect";
    if ( !m_isConnected ) {
        m_socket->open(m_url);
    }
}

void CCNetManager::make_disconnect(void) {
    qDebug() << "make_disconnect";
    if ( m_isConnected ) {
        m_socket->close();
    }
}

void CCNetManager::newMsgForSend(QString msg) {
    qDebug() << "newMsgForSend" << msg;
    if ( m_isConnected ) {
        m_socket->sendTextMessage(msg);
    }
}

void CCNetManager::ws_connected(void) {
    qDebug() << "ws_connected";
    m_isConnected = true;
    emit connected();
}

void CCNetManager::ws_disconnected(void) {
    qDebug() << "ws_disconnected";
    m_isConnected = false;
    emit disconnected();
}

void CCNetManager::ws_error(QAbstractSocket::SocketError error) {
    qDebug() << "ws_error" << error;
}
