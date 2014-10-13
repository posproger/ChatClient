#include "ccchannel.h"

CCChannel::CCChannel(QObject *parent) :
    QObject(parent)
{
}

CCChannel::CCChannel(QString name, QObject *parent) : QObject(parent) {
    m_name = name;
}

void CCChannel::onNewMessage(QString msg) {
    m_text += msg+"\r\n";;
}

