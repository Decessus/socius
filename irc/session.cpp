#include "session.h"

IRC_Session::IRC_Session(QObject* parent) : Irc::Session(parent)
{

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

}

void IRC_Session::on_connected()
{
    g_parent->chantext->append("Connection to irc.paradoxirc.com Successful!\n");
    g_parent->nickLabel->setText(this->nick());

    this->ServerItem = new QTreeWidgetItem(g_parent->serverTree);
    this->ServerItem->setText(0,this->host());
}

void IRC_Session::on_disconnected()
{
    g_parent->chantext->append("\nDisconnected..\n");
}

void IRC_Session::on_msgJoined(const QString& origin, const QString& channel)
{
    if ( origin == this->nick() ) {
        this->ChanList[channel] = new irc_channel;
        this->ChanList[channel]->chanId = new QTreeWidgetItem(this->ServerItem);
        this->ChanList[channel]->chanId->setText(0,channel);
        this->ServerItem->setExpanded(true);
    } else {
        g_parent->chantext->append(origin + " is now chatting in " + channel + "\n");
    }
    this->names(channel);
}

void IRC_Session::on_msgParted(const QString& origin, const QString& channel, const QString& message)
{
    if ( origin == this->nick() ) {

        if(ChanList.contains(channel)) {

            ServerItem->removeChild(ChanList[channel]->chanId);
            delete ChanList[channel];
        }

    } else {

        if(ChanList.contains(channel)) {
            ChanList[channel]->users.removeOne(origin);
            ChanList[channel]->messages.append(origin + " has left the channel (" + message + ")\n");
        }
        names(channel);
    }


}

void IRC_Session::on_msgQuit(const QString& origin, const QString& message)
{
    if ( origin == this->nick() ) {

    } else {
        emit eventRefresh(this->host(),);
        nameAll();
    }
}

void IRC_Session::on_msgNickChanged(const QString& origin, const QString& nick)
{
    if ( origin == this->nick() ) {
        g_parent->chantext->append("<b>Nick Changed</b>\n");
    } else {
        ChanList[channel]->users.removeOne(origin);
    }
    nameAll();
}

void IRC_Session::on_msgModeChanged(const QString& origin, const QString& receiver, const QString& mode, const QString& args)
{

}

void IRC_Session::on_msgTopicChanged(const QString& origin, const QString& channel, const QString& topic)
{
    g_parent->chantext->append(origin + " has changed the topic to: " + topic + "\n");
    g_parent->chantitle->setText(topic);
}

void IRC_Session::on_msgInvited(const QString& origin, const QString& receiver, const QString& channel)
{

}

void IRC_Session::on_msgKicked(const QString& origin, const QString& channel, const QString& nick, const QString& message)
{
    g_parent->chantext->append(origin + " has kicked " + nick + "(" + message + ")");
}

void IRC_Session::on_msgMessageReceived(const QString& origin, const QString& receiver, const QString& message)
{
    g_parent->chantext->append("<" + origin + "> " + message);
}

void IRC_Session::on_msgNoticeReceived(const QString& origin, const QString& receiver, const QString& notice)
{
    g_parent->chantext->append("<b>Notice Recieved From:</b> " + origin + " <b>Message:</b> " + notice);
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

    case 332:
        g_parent->chantitle->setText(params.at(2));
    break;

    case 353:
        QString buff = params.at(3);
        this->ChanList[params.at(2)]->users = buff.split(" ",QString::SkipEmptyParts);
        g_parent->nickList->clear();
        for (int i=0;i<this->ChanList[params.at(2)]->users.size();++i) {
            g_parent->nickList->addItem(new QListWidgetItem(this->ChanList[params.at(2)]->users.at(i)));
        }
    break;

    default:

    }

}

void IRC_Session::on_msgUnknownMessageReceived(const QString& origin, const QStringList& params)
{
    g_parent->chantext->append("<b>Unknown Message Recieved From:</b> " + origin + " <b>Message:</b> ");
    for (int i=0;i<params.size();++i) {
        g_parent->chantext->append(params.at(i) + " ");
    }
    g_parent->chantext->append("\n");
}
