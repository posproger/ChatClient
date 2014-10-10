#include "ccchannel.h"

CCChannel::CCChannel(QObject *parent) :
    QObject(parent)
{
}

void CCChannel::onNewMessage(QString msg) {
    m_text += msg+"\r\n";;
}

