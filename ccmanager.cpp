#include "ccmanager.h"
#include "ccnetmanager.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

CCManager::CCManager(QObject *parent) :
    QObject(parent)
{
    connect(this,SIGNAL(make_connect()),&m_netManager,SLOT(make_connect()));
    connect(this,SIGNAL(make_disconnect()),&m_netManager,SLOT(make_disconnect()));
    connect(&m_netManager,SIGNAL(connected()),this,SLOT(netConnected()));
    connect(&m_netManager,SIGNAL(disconnected()),this,SLOT(netDisconnected()));

    m_curChannel = -1;
    m_loginStage = 0;
}

void CCManager::mkConnect(void) {
    emit make_connect();
}

void CCManager::mkDisconnect(void) {
    emit make_disconnect();
}

void CCManager::netConnected(void) {
    emit connected();
}

void CCManager::netDisconnected(void) {
    m_loginStage=0;
    emit disconnected();
}

void CCManager::mkLogin(QString login, QString pwd, QString email) {
    if ( m_netManager.isConnected() && m_loginStage==0 ) {
        QJsonObject joRequest;
        joRequest["c"]=QString("REG");
        joRequest["l"]=login;
        joRequest["m"]=email;
        joRequest["p"]=pwd;
        QJsonDocument jdRequest(joRequest);
        emit msgForSend(jdRequest.toJson());
    }
}


void CCManager::newMsgReceived(QString msg) {
    ;
}

void CCManager::newMsgForSend(QString msg) {
    if ( m_netManager.isConnected() ) {

    }
}

