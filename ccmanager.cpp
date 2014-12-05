#include "ccmanager.h"
#include "ccnetmanager.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QCryptographicHash>

CCManager::CCManager(QObject *parent) :
    QObject(parent)
{
    connect(this,SIGNAL(make_connect()),&m_netManager,SLOT(make_connect()));
    connect(this,SIGNAL(make_disconnect()),&m_netManager,SLOT(make_disconnect()));
    connect(&m_netManager,SIGNAL(connected()),this,SLOT(netConnected()));
    connect(&m_netManager,SIGNAL(disconnected()),this,SLOT(netDisconnected()));
    connect(this,SIGNAL(msgForSend(QString)),&m_netManager,SLOT(newMsgForSend(QString)));
    connect(&m_netManager,SIGNAL(msgReceived(QString)),this,SLOT(newMsgReceived(QString)));

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
    m_channels.clear();
    m_persChannels.clear();
    m_loginStage=0;
    emit disconnected();
}

void CCManager::mkLogin(QString login, QString pwd, QString email) {
    qDebug() << "CCManager::mkLogin";
    if ( m_netManager.isConnected() && m_loginStage==0 ) {
        QJsonObject joRequest;
        joRequest["c"]=QString("REG");
        joRequest["l"]=login;
        joRequest["m"]=email;
        //joRequest["p"]=pwd;
        joRequest["p"] = QString(QCryptographicHash::hash(pwd.toUtf8(), QCryptographicHash::Sha256).toHex());
        QJsonDocument jdRequest(joRequest);
        m_loginStage=1;
        emit msgForSend(jdRequest.toJson());
    }
}

void CCManager::getChannels(void) {
    if ( m_netManager.isConnected() && m_loginStage==2 ) {
        QJsonObject joRequest;
        joRequest["c"]=QString("GCH");
        QJsonDocument jdRequest(joRequest);
        emit msgForSend(jdRequest.toJson());
    }
}

void CCManager::getPersChannels(void) {
    if ( m_netManager.isConnected() && m_loginStage==2 ) {
        ; /// Пока заглушка на персональные каналы - должна быть проверка онлайн и т.п.
        CCChannel * ch = new CCChannel("Max");
        m_persChannels.insert("Max",ch);
        ch = new CCChannel("Def");
        m_persChannels.insert("Def",ch);
        emit persChannelsReceived(m_persChannels);
    }
}


void CCManager::getChannelHistory(int channel) {
    if ( m_netManager.isConnected() && m_loginStage==2 ) {
        QJsonObject joRequest;
        joRequest["c"]=QString("CHH");
        joRequest["ch"]=channel;
        QJsonDocument jdRequest(joRequest);
        emit msgForSend(jdRequest.toJson());
    }
}

void CCManager::newMsgReceived(QString msg) {
    qDebug() << "CCManager::newMsgReceived\n" << msg;
    if ( !msg.isEmpty() ) {
        QJsonDocument jdoc(QJsonDocument::fromJson(msg.toUtf8()));
        if ( !jdoc.isEmpty() ) {
            QJsonObject jobj = jdoc.object();
            QString cmd = jobj["c"].toString();
            if ( cmd==QString("REG") ) {
                qDebug() << "REG cmd";
                QString res = jobj["res"].toString();
                if ( res=="OK" ) {
                    m_loginStage=2;
                    emit logined();
                    getChannels();
                    getPersChannels();
                } else {
                    m_loginStage=0;
                    emit unlogined();
                }
            } else if ( cmd==QString("GCH") ) {
                qDebug() << "GCH cmd";
                QString res = jobj["res"].toString();
                if ( res=="OK" ) {
                    m_channels.clear();
                    QJsonArray jchs = jobj["chs"].toArray();
                    for ( int i=0; i<jchs.size(); ++i ) {
                        QJsonObject joch = jchs[i].toObject();
                        int index = joch["i"].toInt();
                        QString name = joch["n"].toString();
                        if ( index>0 && !name.isEmpty() ) {
                            CCChannel * ch = new CCChannel(name);
                            m_channels.insert(index,ch);
                            getChannelHistory(index);
                        }
                    }
                    emit channelsReceived(m_channels);
                } else {
                    /// Надо что-то сделать
                }
            } else if ( cmd==QString("MSG") ) {
                qDebug() << "MSG cmd";
                QString res = jobj["res"].toString();
                //QString id = jobj["id"].toString();
                int ch = jobj["ch"].toInt();
                if ( res=="IN" && ch!=0 ) {
                    QString smsg = jobj["msg"].toString();
                    CCChannel* cch = m_channels.value(ch);
                    if ( cch ) {
                        cch->onNewMessage(smsg);
                    }
                } else {
                    /// Пока игнорируем ответы
                    qDebug() << "Ignoring message.\n" << msg;
                }
            } else if ( cmd==QString("CHH") ) {
                qDebug() << "CHH cmd";
                QString res = jobj["res"].toString();
                int chid = jobj["ch"].toInt();
                CCChannel* cch = m_channels.value(chid);
                if ( res=="OK" && cch) {
                    QJsonArray msgs = jobj["msgs"].toArray();
                    for ( int i=0; i<msgs.size(); ++i ) {
                        QString smsg = msgs[i].toString();
                        cch->onNewMessage(smsg);
                    }
                } else {
                    qDebug() << "WARNING! CHH error.\n" << msg;
                }
            } else {
                qDebug() << "WARNING! Unknown message type.\n" << msg;
            }
        } else {
            qDebug() << "WARNING! Invalid format.\n" << msg;
        }
    }
}

void CCManager::newMsgForSend(QString msg, int channel) {
    if ( m_netManager.isConnected() && m_loginStage==2 ) {
        QJsonObject joRequest;
        joRequest["c"]=QString("MSG");
        joRequest["id"]="2";
        joRequest["ch"]=channel;
        joRequest["msg"]=msg;
        QJsonDocument jdRequest(joRequest);
        emit msgForSend(jdRequest.toJson());
    }
}

