#include "irc.h"

/** IRC Event Slots **/

void IRC::event_connected(IRC_Session *Session) {

}

void IRC::event_disconnected(IRC_Session* Session) {

    for(QMap<QString,irc_channel*>::iterator i = Session->ChanList.begin();
    i!=Session->ChanList.end();i++) {
        i.chanId->setText("(" + i.chanId->text() + ")");
        i.text += ("Disconnected from " + host() + "..\n");
    }

    if(Session == activeSession)
        chanText->setText(ChanList[activeChannel]->text);

    else {

        for(QList<IRC_Session>::iterator i = Sessions.begin();i!=Sessions.end();i++) {
            if(i.value() == Session) {

            }
        }

    }

    return;

}

void IRC::event_channel_invite(IRC_Session *Session,QString Nick,QString Channel,QString nChannel) {

    if(Session->IsActive && Session->KnownUsers.contains(Nick)) {

        switch (ClientProfile->ChannelInviteRule) {

        case Status::CI_Block:
            break;

        case Status::CI_Alert:
            Message temp = Session->ChanList[Session->ActiveChannel]->append("INVITE",Nick,
                                                                             "Invited " + Channel + " to " + nChannel);
            chanText->append(ThemeMessage(temp));
            break;

        case Status::CI_Idle:
            Message temp = Session->ChanList[Session->ActiveChannel]->append("INVITE",Nick,
                                                                             "Invited " + Channel + " to " + nChannel);
            chanText->append(ThemeMessage(temp));
            break;

        case Status::CI_Join:
            Session->join(Channel);
            break;
        }

    }

    else {
        if(Session->KnownUsers.contains(Nick)) {

            switch (ClientProfile->ChannelInviteRule) {

            case Status::CI_Block:
                break;

            case Status::CI_Alert:
                Message temp;
                temp.eventType = "INVITE";
                temp.sender = Nick;
                temp.text = "Invited you to " + Channel;
                temp.timestamp = QTime::currentTime().toString("h:mm:ss ap");
                Session->ActiveMessages.enqueue(temp);
                Session->ServerItem->setBackgroundColor(0,eventColor.Invite);
                for(QHash<QString,irc_channel*>::iterator i=Session->ChanList.begin();i!=Session->ChanList.end();i++)
                    Session->ChanList[i.key()]->chanId->setBackgroundColor(0,eventColor.Invite);
                break;

            case Status::CI_Idle:
                Session->ChanList[Channel]->append("INVITE",Nick,"Invited " + Channel + " to " + nChannel);
                Session->ChanList[Channel]->chanId->setBackgroundColor(0,eventColor.Invite);
                break;

            case Status::CI_Join:
                Session->join(Channel);
                break;
            }

            return;
        }
        if(ClientProfile->ChannelInviteRule != Status::CI_Block)
            Session->ChanList["status"]->append("INVITE",Nick,"Invited " + Channel + " to " + nChannel);
    }

    return;
}

void IRC::event_invite(IRC_Session *Session, QString Nick, QString Channel) {
    if(Session->IsActive && Session->KnownUsers.contains(Nick)) {

        switch (ClientProfile->ChannelInviteRule) {

        case Status::CI_Block:
            break;

        case Status::CI_Alert:
            Message temp = Session->ChanList[Session->ActiveChannel]->append("INVITE",Nick,"Invited you to " + Channel);
            chanText->append(ThemeMessage(temp));;
            break;

        case Status::CI_Idle:
            Message temp = Session->ChanList[Session->ActiveChannel]->append("INVITE",Nick,"Invited you to " + Channel);
            chanText->append(ThemeMessage(temp));
            break;

        case Status::CI_Join:
            Session->join(Channel);
            break;
        }

    }

    else {
        if(Session->KnownUsers.contains(Nick)) {

            switch (ClientProfile->ChannelInviteRule) {

            case Status::CI_Block:
                break;

            case Status::CI_Alert:
                Message temp;
                temp.eventType = "INVITE";
                temp.sender = Nick;
                temp.text = "Invited you to " + Channel;
                temp.timestamp = QTime::currentTime().toString("h:mm:ss ap");
                Session->ActiveMessages.enqueue(temp);
                Session->ServerItem->setBackgroundColor(0,eventColor.Invite);
                for(QHash<QString,irc_channel*>::iterator i=Session->ChanList.begin();i!=Session->ChanList.end();i++)
                    Session->ChanList[i.key()]->chanId->setBackgroundColor(0,eventColor.Invite);
                break;

            case Status::CI_Idle:
                Message temp;
                temp.eventType = "INVITE";
                temp.sender = Nick;
                temp.text = "Invited you to " + Channel;
                temp.timestamp = QTime::currentTime().toString("h:mm:ss ap");
                Session->ActiveMessages.enqueue(temp);
                Session->ServerItem->setBackgroundColor(0,eventColor.Invite);
                for(QHash<QString,irc_channel*>::iterator i=Session->ChanList.begin();i!=Session->ChanList.end();i++)
                    Session->ChanList[i.key()]->chanId->setBackgroundColor(0,eventColor.Invite);
                break;

            case Status::CI_Join:
                Session->join(Channel);
                break;
            }

            return;
        }
        if(ClientProfile->ChannelInviteRule != Status::CI_Block)
            Session->ChanList["status"]->append("INVITE",Nick,"Invited you to " + Channel);
    }

    return;
}

void IRC::event_channel_message(IRC_Session *Session, QString Channel, Message Data) {

    if(Session->IsActive && Session->ActiveChannel == Channel)
        chanText->append(ThemeMessage(Data));

    else {
        if(!Data.text.contains(Session->nick()))
            Session->ChanList[Channel]->chanId->setTextColor(0,eventColor.Message);
        else
            Session->ChanList[Channel]->chanId->setTextColor(0,eventColor.Mentioned);
    }

    return;
}

void IRC::event_external_notice(IRC_Session *Session, Message Data) {

    if(Session->IsActive && Session->ActiveChannel == "status")
        chanText->append(ThemeMessage(Data));
    else
        Session->ChanList["status"]->chanId->setTextColor(0,eventColor.Notice);

    return;
}

void IRC::event_joined(IRC_Session *Session, QString Channel, Message Data) {

    if(Session->IsActive && Session->ActiveChannel == Channel)
        chanText->append(ThemeMessage(Data));
    else
        Session->ChanList[Channel]->chanId->setTextColor(0,eventColor.Joined);

    return;
}

void IRC::event_kick(IRC_Session *Session, QString Channel, Message Data) {

    if(Session->IsActive && Session->ActiveChannel == Channel)
        chanText->append(ThemeMessage(Data));
    else
        Session->ChanList[Channel]->chanId->setTextColor(0,eventColor.Kicked);

    return;
}

void IRC::event_kicked(IRC_Session *Session, QString Channel, Message Data) {

    if(Session->IsActive && Session->ActiveChannel == Channel)
        chanText->append(ThemeMessage(Data));
    else
        Session->ChanList[Channel]->chanId->setTextColor(0,eventColor.Kicked);

    Session->ChanList[Channel]->chanId->setText("(" + Channel + ")");

    if(ClientProfile->AutoRejoinOnKick)
        Session->join(Channel);

    return;
}

void IRC::event_mode(IRC_Session *Session, QString Channel, Message Data) {

    if(Session->IsActive && Session->ActiveChannel == Channel)
        chanText->append(ThemeMessage(Data));
    else
        Session->ChanList[Channel]->chanId->setTextColor(0,eventColor.Mode);

    return;

}

void IRC::event_newChannel(IRC_Session *Session, QString Channel) {

    if(!Session->IsActive) {
        for(int i=0;i<sessions.count();i++) {
            if(sessions.at(i)->IsActive)
                sessions.at(i)->IsActive = false;
        }

        Session->IsActive = true;
    }

    RefreshChannelMessages(Session,Channel);
    RefreshChannelUsers(Session,Channel);
    RefreshChannelTopic(Session,Channel);

    return;
}

void IRC::event_nick(IRC_Session *Session, Message Data) {
    QHash<QString,irc_channel*>::iterator i;
    for(i=Session->ChanList.begin();i!=Session->ChanList.end();i++) {
        if(Session->ChanList[i.key()]->users.contains(Data.sender))
            Session->ChanList[i.key()]->chanId->setTextColor(0,eventColor.Nick);
    }

    if(Session->IsActive && Session->ChanList[Session->ActiveChannel]->users.contains(Data.sender))
        chanText->append(ThemeMessage(Data));

    return;
}

void IRC::event_parted(IRC_Session *Session, QString Channel, Message Data) {
    if(Session->IsActive && Session->ActiveChannel == Channel)
        chanText->append(ThemeMessage(Data));
}

void IRC::event_parted_channel(IRC_Session *Session, Message Data) {

    if(Session->IsActive) {
        QHash<QString,irc_channel*>::iterator i;
        i = Session->ChanList.begin();
        RefreshChannelMessages(Session,i.key());
        RefreshChannelUsers(Session,i.key());
        RefreshChannelTopic(Session,i.key());
    }

    return;
}

void IRC::event_private_message(IRC_Session *Session, Message Data) {
    if(Session->IsActive && Session->ActiveChannel == Data.sender)
        chanText->append(ThemeMessage(Data));
    else
        Session->Queries[Data.sender]->chanId->setTextColor(0,eventColor.Message);

    return;
}

void IRC::event_quit(IRC_Session *Session, Message Data) {
    if(Session->IsActive && Session->ChanList[Session->ActiveChannel]->users.contains(Data.sender))
        chanText->append(ThemeMessage(Data));

    for(QHash<QString,irc_channel*>::iterator i=Session->ChanList.begin();i!=Session->ChanList.end();i++) {
        if(Session->ChanList[i.key()]->users.contains(Data.sender))
            Session->ChanList[i.key()]->chanId->setTextColor(0,eventColor.Quit);
    }

    return;
}

void IRC::event_topic(IRC_Session *Session, QString Channel) {
    if(Session->IsActive && Session->ActiveChannel == Channel)
        chanTitle->setText(Session->ChanList[Channel]->topic);

    return;
}

void IRC::event_unknown_notice(IRC_Session *Session, Message Data) {
    if(Session->IsActive && Session->ActiveChannel == "status")
        chanText->append(ThemeMessage(Data));
    else
        Session->ChanList["status"]->chanId->setTextColor(0,eventColor.Notice);

    return;
}
