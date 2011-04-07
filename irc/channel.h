#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>
#include <QTreeWidgetItem>
#include <QListWidgetItem>
#include <QTime>
#include <QFile>
#include <QQueue>
#include <QPixmap>

#include "utilities.h"

class private_message {
public:
    explicit irc_channel(QString channel,QObject* parent = 0);
    explicit ~irc_channel();
    QTreeWidgetItem *chanId;
    QQueue<Message> getMessages() { return messages; }
    Message append(QString event, QString origin, QString message);
private:
    QQueue<Message> messages;
};

class irc_channel {
public:
    explicit irc_channel();
    explicit irc_channel(QString channel,QObject* parent = 0);
    explicit ~irc_channel();

    QString topic;
    QTreeWidgetItem *chanId;
    QByteArray modes;
    QStringList users;

    bool isVoiceChannel();

    QQueue<Message> getMessages() { return messages; }
    Message append(QString event, QString origin, QString message);
private:
    QQueue<Message> messages;
};

#endif // CHANNEL_H
