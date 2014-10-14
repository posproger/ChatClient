#include "ccchannel.h"
#include <QDebug>

CCChannel::CCChannel(QObject *parent) :
    QObject(parent)
{
}

CCChannel::CCChannel(QString name, QObject *parent) : QObject(parent) {
    m_name = name;
}

void CCChannel::onNewMessage(QString msg) {
    qDebug() << "CCChannel::onNewMessage" << m_name << msg;
    if ( !msg.isEmpty() ) {
        m_text += msg+"\r\n";
        emit newMessage(msg);
    }
}

