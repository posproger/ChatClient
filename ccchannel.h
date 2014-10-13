#ifndef CCCHANNEL_H
#define CCCHANNEL_H

#include <QObject>

class QString;

class CCChannel : public QObject
{
    Q_OBJECT
public:
    explicit CCChannel(QObject *parent = 0);
    explicit CCChannel(QString name, QObject *parent = 0);

    QString getName(void) { return m_name; }
    QString getText(void) { return m_text; }

signals:

public slots:
    void onNewMessage(QString msg);

private:
    QString m_name;
    QString m_text;

};

#endif // CCCHANNEL_H
