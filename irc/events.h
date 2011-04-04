#ifndef EVENTS_H
#define EVENTS_H

#include <QString>
#include <QTime>
#include <QDate>
#include <QStringList>

class irc_events {
private:
    struct message{
        QString eventType,sender,timestamp;
    };

    QQueue<message> toLog;
    QQueue<message> messages;
    unsigned int bufferSize;

public:
    //Measured in kilobytes
    explicit irc_events(unsigned int bufferSize = 1024);
    explicit ~irc_events();
    QString toRaw();
    QString toBasic();
    QString toFormatted();
    void append(QString event,QString origin,QTime timestamp = QTime::currentTime());
    void setTemplate(QString event,QString value,bool replace = true);
};

#endif // EVENTS_H
