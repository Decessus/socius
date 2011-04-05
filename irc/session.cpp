#include "session.h"

IRC_Session::IRC_Session(QObject* parent,QTreeWidget* sParent) : Irc::Session(parent) {

    ChanList.insert("status",new irc_channel(this));
    ServerItem = new QTreeWidgetItem(sParent);
    ServerItem->setText(0,"< new >");
    Message temp = ChanList["status"]->append("INIT","*","Initializing protocol..");
    emit init_message(this,temp);

    connect(this, SIGNAL(connected()), SLOT(on_connected()));
    connect(this, SIGNAL(disconnected()), SLOT(on_disconnected()));
    connect(this, SIGNAL(msgQuit(QString, QString)), SLOT(on_msgQuit(QString, QString)));
    connect(this, SIGNAL(msgJoined(QString, QString)), SLOT(on_msgJoined(QString, QString)));
    connect(this, SIGNAL(msgNickChanged(QString, QString)), SLOT(on_msgNickChanged(QString, QString)));
    connect(this, SIGNAL(msgParted(QString, QString, QString)), SLOT(on_msgParted(QString, QString, QString)));
    connect(this, SIGNAL(msgInvited(QString, QString, QString)), SLOT(on_msgInvited(QString, QString, QString)));
    connect(this, SIGNAL(msgCtcpReplyReceived(QString, QString)), SLOT(on_msgCtcpReplyReceived(QString, QString)));
    connect(this, SIGNAL(msgCtcpRequestReceived(QString, QString)), SLOT(on_msgCtcpRequestReceived(QString, QString)));
    connect(this, SIGNAL(msgTopicChanged(QString, QString, QString)), SLOT(on_msgTopicChanged(QString, QString, QString)));
    connect(this, SIGNAL(msgNoticeReceived(QString, QString, QString)), SLOT(on_msgNoticeReceived(QString, QString, QString)));
    connect(this, SIGNAL(msgKicked(QString, QString, QString, QString)), SLOT(on_msgKicked(QString, QString, QString, QString)));
    connect(this, SIGNAL(msgMessageReceived(QString, QString, QString)), SLOT(on_msgMessageReceived(QString, QString, QString)));
    connect(this, SIGNAL(msgUnknownMessageReceived(QString, QStringList)), SLOT(on_msgUnknownMessageReceived(QString, QStringList)));
    connect(this, SIGNAL(msgCtcpActionReceived(QString, QString, QString)), SLOT(on_msgCtcpActionReceived(QString, QString, QString)));
    connect(this, SIGNAL(msgModeChanged(QString, QString, QString, QString)), SLOT(on_msgModeChanged(QString, QString, QString, QString)));
    connect(this, SIGNAL(msgNumericMessageReceived(QString, uint, QStringList)), SLOT(on_msgNumericMessageReceived(QString, uint, QStringList)));

}

IRC_Session::IRC_Session(server_profile *profile, QObject *parent, QTreeWidget *sParent)  : Irc::Session(parent) {

    ChanList.insert("status",new irc_channel(this));
    ServerItem = new QTreeWidgetItem(sParent);
    ServerItem->setText(0,profile->HostName);

    Message temp = ChanList["status"]->append("INIT","*","Initializing protocol..");
    emit init_message(this,temp);

    connect(this, SIGNAL(connected()), SLOT(on_connected()));
    connect(this, SIGNAL(disconnected()), SLOT(on_disconnected()));
    connect(this, SIGNAL(msgQuit(QString, QString)), SLOT(on_msgQuit(QString, QString)));
    connect(this, SIGNAL(msgJoined(QString, QString)), SLOT(on_msgJoined(QString, QString)));
    connect(this, SIGNAL(msgNickChanged(QString, QString)), SLOT(on_msgNickChanged(QString, QString)));
    connect(this, SIGNAL(msgParted(QString, QString, QString)), SLOT(on_msgParted(QString, QString, QString)));
    connect(this, SIGNAL(msgInvited(QString, QString, QString)), SLOT(on_msgInvited(QString, QString, QString)));
    connect(this, SIGNAL(msgCtcpReplyReceived(QString, QString)), SLOT(on_msgCtcpReplyReceived(QString, QString)));
    connect(this, SIGNAL(msgCtcpRequestReceived(QString, QString)), SLOT(on_msgCtcpRequestReceived(QString, QString)));
    connect(this, SIGNAL(msgTopicChanged(QString, QString, QString)), SLOT(on_msgTopicChanged(QString, QString, QString)));
    connect(this, SIGNAL(msgNoticeReceived(QString, QString, QString)), SLOT(on_msgNoticeReceived(QString, QString, QString)));
    connect(this, SIGNAL(msgKicked(QString, QString, QString, QString)), SLOT(on_msgKicked(QString, QString, QString, QString)));
    connect(this, SIGNAL(msgMessageReceived(QString, QString, QString)), SLOT(on_msgMessageReceived(QString, QString, QString)));
    connect(this, SIGNAL(msgUnknownMessageReceived(QString, QStringList)), SLOT(on_msgUnknownMessageReceived(QString, QStringList)));
    connect(this, SIGNAL(msgCtcpActionReceived(QString, QString, QString)), SLOT(on_msgCtcpActionReceived(QString, QString, QString)));
    connect(this, SIGNAL(msgModeChanged(QString, QString, QString, QString)), SLOT(on_msgModeChanged(QString, QString, QString, QString)));
    connect(this, SIGNAL(msgNumericMessageReceived(QString, uint, QStringList)), SLOT(on_msgNumericMessageReceived(QString, uint, QStringList)));

    temp = ChanList["status"]->append("INIT","*","Setting up server environment..");
    emit init_message(this,temp);

    setAutoJoinChannels(profile->AutoJoinChannels);
    setRealName(profile->RealName);

    if(!profile->DetermineBestHost || profile->ServerData.count()<=1) {
        if(profile->DefaultServer.isEmpty()) {
            QHash<QString,int>::iterator i = profile->ServerData.begin();
            temp = ChanList["status"]->append("INIT","*","Could not determine best host. Using " +
                                              i.key() + " instead.");
            emit init_message(this,temp);

            setHost(i.key());

            if(profile->ServerData.value(i.key()))
                setPort(profile->ServerData.value(i.key()));
            else {
                if(!profile->UseSSL) {
                    setPort(6667);
                }
                else
                    setPort(7000);
            }
        }

        else {
            setHost(profile->DefaultServer);

            if(profile->ServerData.value(profile->DefaultServer))
                setPort(profile->ServerData.value(profile->DefaultServer));
            else {
                if(!profile->UseSSL) {
                    setPort(6667);
                }
                else
                    setPort(7000);
            }
        }
    }

    //Determine best host by average connection time
    else {
        QTime *PingTimer = new QTime(),*DelayTimer = new QTime();
        QHash<int,QString> PingResponses;
        PingTimer->start();
        DelayTimer->start();

        temp = ChanList["status"]->append("INIT","*","Determining the fastest connection response on the network..");
        emit init_message(this,temp);

        /**
          Since Linux machines will require root privs to use ICMP, we will
            measure the ammount of time to receive a connection response, rather
            than send an ICMP Ping.
          **/
        for(int p=0;p<5;p++) {
            DelayTimer->restart();

            for(QHash<QString,int>::iterator i = ServerData.begin();i!=ServerData.end();i++) {
                QTcpSocket *PingSocket = new QTcpSocket(this);
                PingTimer->restart();
                PingSocket->connectToHost(i.key(),i.value());
                PingResponses.insert(PingTimer->elapsed(),i.key());
                PingSocket->disconnectFromHost();
                delete PingSocket;
            }

            if(DelayTimer->elapsed()<1000)
                QSleep::msleep(1000-DelayTimer->elapsed());
        }

        QPair<int,QString> BestResponse;
        for(QHash<int,QString>::iterator i=PingResponses.begin();i!=PingResponses.end();i++) {
            if(i=PingResponses.begin()) {
                BestResponse.first = i.key();
                BestResponse.second = i.value();
            }

            else {
                if(BestResponse.first>i.key()) {
                    BestResponse = i.key();
                    BestResponse.second = i.value();
                }
            }
        }

        if(!BestResponse.second.isEmpty()) {
            temp = ChanList["status"]->append("INIT","*","Determined best host as " +
                                              BestResponse.second + "..");
            emit init_message(this,temp);

            setHost(BestResponse.second);

            if(profile->ServerData.value(BestResponse.second))
                setPort(profile->ServerData.value(BestResponse.second));
            else {
                if(!profile->UseSSL) {
                    setPort(6667);
                }
                else
                    setPort(7000);
            }
        }

        else {
            if(profile->DefaultServer.isEmpty()) {
                QHash<QString,int>::iterator i = profile->ServerData.begin();
                temp = ChanList["status"]->append("INIT","*","Could not determine best host. Using " +
                                                  i.key() + " instead.");
                emit init_message(this,temp);

                setHost(i.key());

                if(profile->ServerData.value(i.key()))
                    setPort(profile->ServerData.value(i.key()));
                else {
                    if(!profile->UseSSL) {
                        setPort(6667);
                    }
                    else
                        setPort(7000);
                }
            }

            else {
                setHost(profile->DefaultServer);

                if(profile->ServerData.value(profile->DefaultServer))
                    setPort(profile->ServerData.value(profile->DefaultServer));
                else {
                    if(!profile->UseSSL) {
                        setPort(6667);
                    }
                    else
                        setPort(7000);
                }
            }
        }
    }

}

void IRC_Session::on_connected() {
    ChanList["status"]->append("CONNECTED",host(),"Connection Established..");
    emit event_connected(this);
    return;
}



void IRC_Session::on_disconnected() {

    for(QHash<QString,irc_channel*>::iterator i = ChanList.begin();i!=ChanList.end();i++) {
        ChanList[i.key()]->chanId->setText("(" + i.value()->chanId->setText + ")");
        ChanList[i.key()]->append("DISCONNECTED",host(),"");
    }

    emit event_disconnected(this);
    return;

}



void IRC_Session::on_msgJoined(const QString& origin, const QString& channel) {

    if ( origin == nick() ) {
        ChanList.insert(channel,new irc_channel(channel,ServerItem));
        ChanList[channel]->append("JOINED",origin,"Now talking in " + channel);
        names(channel);
        emit event_newChannel(this,channel);
    }

    else {
        ChanList[channel]->append("JOINED",origin,"");
        ChanList[channel]->users.append(origin);
        emit event_joined(this,channel);
    }

}



void IRC_Session::on_msgParted(const QString& origin,const QString& channel,const QString& message) {

    if (origin == nick()) {
        if(ChanList.contains(channel)) {
            delete ChanList.take(channel);
            emit event_parted_channel(this,channel);
        }
    }

    else {
        if(ChanList.contains(channel)) {
            ChanList[channel]->append("PARTED",origin,message);
            ChanList[channel]->users.removeOne(origin);
            emit event_parted(this,channel);
        }
    }

}



void IRC_Session::on_msgQuit(const QString& origin, const QString& message) {

    for(QHash<QString,irc_channel*>::iterator i = ChanList.begin();i!=ChanList.end();i++) {
        if(ChanList[i.key()]->users.contains(origin))
            ChanList[i.key()]->append("QUIT",origin,message);
    }

    emit event_quit(this,origin);

}



void IRC_Session::on_msgNickChanged(const QString& origin, const QString& newNick) {

    for(QHash<QString,irc_channel*>::iterator i = ChanList.begin();i!=ChanList.end();i++) {

        if(ChanList[i.key()]->users.contains(origin)) {
            if(newNick == nick())
                ChanList[i.key()]->append("NICK",origin,"You are now chatting as " + newNick);
            else
                ChanList[i.key()]->append("NICK",origin,"Is now chatting as " + newNick);

            ChanList[i.key()]->users.removeOne(origin);
            ChanList[i.key()]->users.append(newNick);
        }

    }

    emit event_nick(this,newNick);
}



void IRC_Session::on_msgModeChanged(const QString& origin, const QString& receiver, const QString& mode, const QString& args) {

    if(ChanList.contains(receiver)) {
        ChanList[receiver]->append("MODE",origin,"Has set " + mode + "[" + args + "] on" + receiver);
        emit event_mode(this,receiver);
    }

}



void IRC_Session::on_msgTopicChanged(const QString& origin, const QString& channel, const QString& topic) {

    if(ChanList.contains(channel)) {
        ChanList[channel].append("TOPIC",origin, "Changed the topic to: " + topic);
        ChanList[channel]->topic = topic;
        emit event_topic(this,channel);
    }

}



void IRC_Session::on_msgInvited(const QString& origin, const QString& receiver, const QString& channel) {

    if(receiver.startsWith('#')) {
        if(ChanList.contains(receiver))
            ChanList[receiver]->append("CHANINVITE",origin,channel);

        emit event_channel_invite(this,origin,channel);
    }
    else
        emit event_invite(this,origin,channel);

}



void IRC_Session::on_msgKicked(const QString& origin, const QString& channel, const QString& nick, const QString& message) {

    if(ChanList.contains(channel)) {
        ChanList[channel]->append("KICK",origin,message);
        if(nick == nick())
            emit event_kicked(this,origin,channel);
        else
            emit event_kick(this,channel);
    }

}



void IRC_Session::on_msgMessageReceived(const QString& origin, const QString& receiver, const QString& message) {

    if(receiver == nick()) {
        if(!Queries.contains(origin))
            Queries.insert(origin,new private_message(origin,ServerItem));

        Message temp = Queries[origin]->append(origin,message);
        emit event_private_message(this,temp);
    }

    else {
        if(ChanList.contains(receiver)) {
            Message temp = ChanList[receiver]->append("MESSAGE",origin,message);
            emit event_channel_message(this,receiver,temp);
        }
    }

}



void IRC_Session::on_msgNoticeReceived(const QString& origin, const QString& receiver, const QString& notice) {

    Message temp;
    temp.text = notice;
    temp.sender = origin;
    temp.eventType = "NOTICE";
    temp.timestamp = QTime::currentTime();

    if(receiver == nick()) {
        if(!UserData.contains(origin))
            emit event_unknown_notice(this,temp);

        else {
            for(QHash<QString,irc_channel*>::iterator i = ChanList.begin();i!=ChanList.end();i++) {
                if(ChanList[i.key()]->users.contains(origin)) {
                    isInChan = true;
                    break;
                }
            }
            if(isInChan)
                emit even_notice(this,temp);
            else
                emit event_external_notice(this,temp);
        }

    }

    return;
}

void IRC_Session::on_msgCtcpRequestReceived(const QString& origin, const QString& request) {
    Message temp;
    temp.text = request;
    temp.sender = origin;
    temp.eventType = "CTCP-REQUEST";
    temp.timestamp = QTime::currentTime();

    emit ctcp_request(this,temp);
    return;
}

void IRC_Session::on_msgCtcpReplyReceived(const QString& origin, const QString& reply) {
    Message temp;
    temp.text = reply;
    temp.sender = origin;
    temp.eventType = "CTCP-REPLY";
    temp.timestamp = QTime::currentTime();

    emit ctcp_reply(this,temp);
    return;
}

void IRC_Session::on_msgCtcpActionReceived(const QString& origin, const QString& receiver, const QString& action) {
    Message temp;
    temp.text = action;
    temp.sender = origin;
    temp.eventType = "CTCP-ACTION";
    temp.extraParams.append("receiver:" + receiver);
    temp.timestamp = QTime::currentTime();

    emit ctcp_action(this,temp);
    return;
}

void IRC_Session::on_msgNumericMessageReceived(const QString& origin, uint code, const QStringList& params)
{
    switch(code) {

        /* Topic change already handled by libircclient-qt, was this a possible fix?
    case 332:

        if(ChanList[origin]) {
            ChanList[origin]->topic = params.at(2);

            if((activeChannel->chanId->text() == origin) && isActive)
                chanTitle->setText(params.at(2));

        }

    break;
    */

    case 353:
        if(ChanList.contains(params.at(2))) {
            QString buff(params.at(3));
            ChanList[params.at(2)]->users = buff.split(" ",QString::SkipEmptyParts);
            emit event_names(this,params.at(2));
        }
    break;

    default:
        Message temp;
        temp.sender = origin;
        temp.eventType = "NUM_" + code;
        temp.extraParams.append(params);
        temp.timestamp = QTime::currentTime();
        NumericMessages.enqueue(temp);
    }

}

void IRC_Session::on_msgUnknownMessageReceived(const QString& origin, const QStringList& params) {
    Message temp;
    temp.sender = origin;
    temp.eventType = "UNKNOWN";
    temp.extraParams.append(params);
    temp.timestamp = QTime::currentTime();
    UnknownMessages.enqueue(temp);

    emit event_unknown(this,temp);
    return;
}
