#ifndef CCCHANNEL_H
#define CCCHANNEL_H

#include <QObject>

class QString;

class CCChannel : public QObject
{
    Q_OBJECT
public:
    explicit CCChannel(QObject *parent = 0);

signals:

public slots:
    void onNewMessage(QString msg);

private:
    QString m_text;

};

#endif // CCCHANNEL_H
