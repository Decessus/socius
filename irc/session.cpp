#include "session.h"

IRC_Session::IRC_Session(QObject* parent,QRect* frameSize,QTreeWidget* sParent) : Irc::Session(parent)
{

    sessionFrame = new QFrame(parent);
    sessionFrame->setGeometry(frameSize);
    sessionFrame->setVisible(true);

    ChanList["status"] = new irc_channel;
    ChanList["status"]->chanId = new QTreeWidgetItem(sParent);
    ChanList["status"]->chanId->setText(0,"< new >");

    chanText = new QTextEdit(sessionFrame);
    chanText->setGeometry(0,22,sessionFrame->width()-145,sessionFrame->height()-44);
    chanText->setVisible(true);

    chanInput = new QLineEdit(sessionFrame);
    chanInput->setGeometry(102,sessionFrame->height()-22,22,sessionFrame->width()-102);
    chanInput->setVisible(true);

    nickButton = new QPushButton(sessionFrame);
    nickButton->setGeometry(0,sessionFrame->height()-22,22,102);
    nickButton->setVisible(true);

    nickList = new QListWidget(sessionFrame);
    nickList->setGeometry(sessionFrame->width()-145,22,sessionFrame->height()-44,145);
    nickList->setVisible(true);

    chanTitle = new QLineEdit(sessionFrame);
    chanTitle->setGeometry(0,0,22,sessionFrame->width());
    chanTitle->setVisible(true);

    connect(this, SIGNAL(connected()), SLOT(on_connected()));
    connect(this, SIGNAL(disconnected()), SLOT(on_disconnected()));
    connect(this, SIGNAL(msgJoined(QString, QString)), SLOT(on_msgJoined(QString, QString)));
    connect(this, SIGNAL(msgParted(QString, QString, QString)), SLOT(on_msgParted(QString, QString, QString)));
    connect(this, SIGNAL(msgQuit(QString, QString)), SLOT(on_msgQuit(QString, QString)));
    connect(this, SIGNAL(msgNickChanged(QString, QString)), SLOT(on_msgNickChanged(QString, QString)));
    connect(this, SIGNAL(msgModeChanged(QString, QString, QString, QString)), SLOT(on_msgModeChanged(QString, QString, QString, QString)));
    connect(this, SIGNAL(msgTopicChanged(QString, QString, QString)), SLOT(on_msgTopicChanged(QString, QString, QString)));
    connect(this, SIGNAL(msgInvited(QString, QString, QString)), SLOT(on_msgInvited(QString, QString, QString)));
    connect(this, SIGNAL(msgKicked(QString, QString, QString, QString)), SLOT(on_msgKicked(QString, QString, QString, QString)));
    connect(this, SIGNAL(msgMessageReceived(QString, QString, QString)), SLOT(on_msgMessageReceived(QString, QString, QString)));
    connect(this, SIGNAL(msgNoticeReceived(QString, QString, QString)), SLOT(on_msgNoticeReceived(QString, QString, QString)));
    connect(this, SIGNAL(msgCtcpRequestReceived(QString, QString)), SLOT(on_msgCtcpRequestReceived(QString, QString)));
    connect(this, SIGNAL(msgCtcpReplyReceived(QString, QString)), SLOT(on_msgCtcpReplyReceived(QString, QString)));
    connect(this, SIGNAL(msgCtcpActionReceived(QString, QString, QString)), SLOT(on_msgCtcpActionReceived(QString, QString, QString)));
    connect(this, SIGNAL(msgNumericMessageReceived(QString, uint, QStringList)), SLOT(on_msgNumericMessageReceived(QString, uint, QStringList)));
    connect(this, SIGNAL(msgUnknownMessageReceived(QString, QStringList)), SLOT(on_msgUnknownMessageReceived(QString, QStringList)));

    connect(sPartent,SIGNAL(itemClicked(QTreeWidgetItem *,int)),this,SLOT(itemClicked(QTreeWidgetItem *,int)));

}



void IRC_Session::itemClicked(QTreeWidgetItem *item, int i) {

    //Is the item clicked a child of this sessions server?
    if(item->parent() == ChanList["status"]->chanId) {

        if(!isActive)
            isActive = true;

        if(ChanList[item->text()])
            switchChannels(item->text());

        else {
            //Create and report error data
            return;
        }

    }

    else {

        if(isActive)
            isActive = false;

        return;
    }

}



void IRC_Session::switchChannels(QString channel) {

    if(ChanList[channel]) {

        activeChannel = channel;
        chanTitle->setText(ChanList[channel]->topic);
        chanText->setText(ChanList[channel]->text);

        //Delete the physical QListWidgetItems from the nick list
        for(int i=0;i<nickList->count();i++)
            delete nickList->item(i);

        //Add the new QListWidgetItems
        for(int i=0;i<this->ChanList[channel]->users.size();i++)
            nickList->addItem(new QListWidgetItem(ChanList[channel]->users.at(i)));

        return;

    }

    else {
        //Create and report error data
        return;
    }

}



void IRC_Session::on_connected()
{
    if(ChanList.contains("status")) {
        ChanList["status"]->text.append("Connection to " + host() + " established..\n");
        ChanList["status"]->chanId->setText(0,host());

        if(isActive && (ChanList["status"]->chanId == activeChannel->chanId))
            chanText = ChanList["status"]->text;
        //else
        //    ChanList["status"]->chanId->setTextColor();
    }

}



void IRC_Session::on_disconnected()
{

    for(QMap<QString,irc_channel*>::iterator i = ChanList.begin();i!=ChanList.end();i++) {
        i.chanId->setText("(" + ChanList["status"]->chanId->child(i)->text() + ")");
        i.text += ("Disconnected from " + host() + "..\n");
    }

    if(isActive)
        chanText->setText(ChanList[activeChannel]->text);

    return;

}



void IRC_Session::on_msgJoined(const QString& origin, const QString& channel)
{
    if ( origin == this->nick() ) {

        ChanList[channel] = new irc_channel;
        ChanList[channel]->chanId = new QTreeWidgetItem(ChanList["status"]->chanId);
        ChanList[channel]->chanId->setText(0,channel);

        if(isActive) {

            ChanList["status"]->chanId->setExpanded(true);
            switchChannels(channels);
            ChanList[channel]->text.append("Now talking in channel " + channel + "..\n");
            chanText->setText(ChanList[channel]->text);
        }

    } else {

        ChanList[channel]->text.append(origin + " is now chatting in " + channel + "\n");

        if(isActive && channel == activeChannel)
            chanText->setText(ChanList[channel]->text);

    }
    this->names(channel);
}



void IRC_Session::on_msgParted(const QString& origin, const QString& channel, const QString& message)
{
    if ( origin == nick() ) {

        if(ChanList.contains(channel)) {

            ChanList["status"]->chanId->removeChild(ChanList[channel]->chanId);
            delete ChanList[channel];

        }

    } else {

        if(ChanList.contains(channel)) {
            ChanList[channel]->users.removeOne(origin);
            ChanList[channel]->text.append(origin + " has left the channel (" + message + ")\n");
        }
        names(channel);
    }

}



void IRC_Session::on_msgQuit(const QString& origin, const QString& message)
{
    if ( origin == this->nick() ) {

    }

    else {

        for(QMap<QString,irc_channel*>::iterator i = ChanList.begin();i!=ChanList.end();i++) {
            names(i.key());
            ChanList[i.key()]->text.append(origin + " has quit (" + message + ")\n");
        }

        if(isActive)
            chanText->setText(ChanList[activeChannel]->text);

    }

}



void IRC_Session::on_msgNickChanged(const QString& origin, const QString& newNick)
{
    if (newNick == nick() && isActive) {

        nickButton->setText(nick);
        ChanList[activeChannel]->text.append("Your nick has been changed to: " + nick + "..\n");
        chanText->setText(ChanList[activeChannel]->text);

    }

    for(QMap<QString,irc_channel*>::iterator i = ChanList.begin();i!=ChanList.end();i++) {

        if(ChanList[i.key()]->users.contains(origin)) {
            names(i.key());

            if(newNick != nick())
                ChanList[i.key()]->text.append("User " + origin + " has changed their nick to: " + nick + "..\n");
        }

    }

    if(isActive)
        chanText->setText(ChanList[activeChannel]->text);
}



void IRC_Session::on_msgModeChanged(const QString& origin, const QString& receiver, const QString& mode, const QString& args)
{

    if(receiver.startsWith('#') && ChanList.contains(receiver)) {

        ChanList[receiver]->text.append(origin + " has set channel mode(s): " + mode);

        if(!args.isEmpty())
            ChanList[receiver]->text.append(" (" + args + ")\n");
        else
            ChanList[receiver]->text.append("\n");

        if(isActive && receiver == activeChannel)
            chanText->setText(ChanList[activeChannel]->text);

    }

    else {

        for(QMap<QString,irc_channel*>::iterator i = ChanList.begin();i!=ChanList.end();i++) {

            if(ChanList[i.key()]->users.contains(receiver)) {
                ChanList[i.key()]->text.append(origin + " has set channel mode(s): " + mode);

                if(!args.isEmpty())
                    ChanList[i.key()]->text.append(" (" + args + ") on user " + receiver + "\n");
                else
                    ChanList[i.key()]->text.append(" on user " + receiver + "\n");

                if(isActive && i.key() == activeChannel)
                    chanText->setText(ChanList[activeChannel]->text);
            }

        }

    }

}



void IRC_Session::on_msgTopicChanged(const QString& origin, const QString& channel, const QString& topic)
{

    if(ChanList.contains(channel)) {

        ChanList[channel]->text.append(origin + " has changed the topic to: " + topic + "\n");
        ChanList[channel]->topic = topic;

        if(isActive && channel == activeChannel) {
            chanText->setText(ChanList[activeChannel]->text);
            chanTitle->setText(ChanList[channel]->topic);
        }

    }

}



void IRC_Session::on_msgInvited(const QString& origin, const QString& receiver, const QString& channel)
{

}



void IRC_Session::on_msgKicked(const QString& origin, const QString& channel, const QString& nick, const QString& message)
{
    if(ChanList.contains(channel)) {
        ChanList[channel]->text.append(origin + " has kicked " + nick + "(" + message + ")");

        if(isActive && channel == activeChannel)
            chanText->setText(ChanList[channel]->text);

    }
}



void IRC_Session::on_msgMessageReceived(const QString& origin, const QString& receiver, const QString& message)
{
    if(ChanList.contains(channel)) {
        ChanList[channel]->text.append("<" + origin + "> " + message);

        if(isActive && channel == activeChannel)
            chanText->setText(ChanList[channel]->text);
    }
}



void IRC_Session::on_msgNoticeReceived(const QString& origin, const QString& receiver, const QString& notice)
{
    bool isInChan = false;

    for(QMap<QString,irc_channel*>::iterator i = ChanList.begin();i!=ChanList.end();i++) {
        if(ChanList[i.key()]->users.contains(origin)) {
            ChanList[i.key()]->text.append("<b>Notice Recieved From:</b> " + origin +
                                           " <b>Message:</b> " + notice + "\n");

            if(!isInChan)
                isInChan = true;
        }
    }

    if(!isInChan)
        ChanList[i.key()]->text.append("<b>Notice Recieved From:</b> " + origin + " <b>Message:</b> " + notice +
                                       " (User is not in any channels as you)\n");

    if(isActive && ChanList[activeChannel]->users.contains(origin))
        chanText->setText(ChanList[activeChannel]->text);

}

void IRC_Session::on_msgCtcpRequestReceived(const QString& origin, const QString& request)
{

}

void IRC_Session::on_msgCtcpReplyReceived(const QString& origin, const QString& reply)
{

}

void IRC_Session::on_msgCtcpActionReceived(const QString& origin, const QString& receiver, const QString& action)
{
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
        if(ChanList[params.at(2)]) {

            QString buff = params.at(3);
            ChanList[params.at(2)]->users = buff.split(" ",QString::SkipEmptyParts);

            if((activeChannel->chanId->text() == origin) && isActive) {
                nickList->clear();
                for (int i=0;i<this->ChanList[params.at(2)]->users.size();++i) {
                    nickList->addItem(new QListWidgetItem(ChanList[params.at(2)]->users.at(i)));
                }
            }

        }
    break;

    default:

    }

}

void IRC_Session::on_msgUnknownMessageReceived(const QString& origin, const QStringList& params)
{
    ChanList["status"]->text += "<b>Unknown Message Recieved From:</b> " + origin + " <b>Message:</b> ";

    if(!params.isEmpty()) {
        for (int i=0;i<params.size();++i)
            ChanList["status"]->text += append(params.at(i) + " ");
    }

    ChanList["status"]->text += "\n";

}
