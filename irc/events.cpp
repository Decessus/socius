#include "events.h"

irc_events::irc_events(unsigned int bufferSize = 1024) {

}

void irc_events::append(QString event, QString origin, QTime timestamp = QTime::currentTime()) {
    message temp;
    temp.eventType = event.toUpper();
    temp.sender = origin;
    temp.timestamp = timestamp;
    messages.enqueue(temp);

    //Check data usage after append
    while((messages.size()/1024)>bufferSize) {
        //Logging is handled on a timed event in the IRC class, default is 3 minutes
        if(Logging)
            toLog.append(messages.dequeue());
        else
            messages.dequeue();
    }

}

QQueue irc_events::getEvents() {
    return messages;
}
