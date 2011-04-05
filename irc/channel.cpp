#include "channel.h"

irc_channel::irc_channel(QString channel,QObject* parent = 0,QTreeWidgetItem* Sparent = 0) {
    chanId = new QTreeWidgetItem(Sparent);
    chanId->setText(0,channel);
    append("JOIN","*","Now talking in " + channel);
}

irc_channel::irc_channel() {

}

irc_channel::~irc_channel() {
    delete chanId;
    users.clear();
    topic.clear();
    modes.clear();
    messages.clear();
}

Message irc_channel::append(QString event, QString origin,QString data) {
    Message temp;
    temp.text = data;
    temp.sender = origin;
    temp.eventType = event.toUpper();
    temp.timestamp = QTime::currentTime().toString("h:mm:ss ap");

    messages.enqueue(temp);

    //Check data usage after append
    while((messages.size()/1024)>bufferSize)
        messages.dequeue();

    return temp;
}

private_message::private_message() {

}

private_message::~private_message() {
    delete chanId;
    messages.clear();
}

Message private_message::append(QString origin,QString message) {
    Message temp;
    temp.text = data;
    temp.sender = origin;
    temp.eventType = "PRIVMSG";
    temp.timestamp = QTime::currentTime().toString("h:mm:ss ap");

    messages.enqueue(temp);

    //Check data usage after append
    while((messages.size()/1024)>bufferSize)
        messages.dequeue();

    return temp;
}
