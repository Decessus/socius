#ifndef SESSION_H
#define SESSION_H

#include <QTextEdit>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QTcpSocket>
#include <QThread>
#include <QHash>

#include "channel.h"
#include "utilities.h"

struct server_profile {
    QString HostName,RealName,Ident,DefaultServer;
    QHash<QString,QString> NickData; //nick,password
    QHash<QString,int> ServerData; //server,port
    QStringList ConnectCommands,AutoJoinChannels;
};

class IRC_Session : public Irc::Session {
    Q_OBJECT

    enum OperStatus {
        NoOper,HelpOper,LocalOper,GlobalOper,
        TechnicalAdmin,NetworkAdmin,ServerAdmin
    };

    public:
        friend class IRC;
        ~IRC_Session();
        explicit IRC_Session(QTreeWidget* sParent,QObject* parent = 0);
        explicit IRC_Session(server_profile* profile = 0,QObject* parent = 0,QTreeWidget* sParent = 0);

    private:
        QByteArray Modes;
        QString HostNetwork;
        server_profile *profile;
        QTreeWidgetItem *ServerItem;
        QByteArray VoiceServerAddress;
        /** OOB(Out of Bounds) Messages
          A message was received outside of user scope
            Example: Recieved a message from a channel you are not
              in, or from a channel that was not properly created by Socius.
          **/
        QQueue<Message> OOB_Messages;
        QQueue<Message> NumericMessages;
        QQueue<Message> UnknownMessages;
        QQueue<QString> ConnectCommands;

        QHash<QString,user> UserData;
        QHash<QString,QString> NickData; //nick,password
        QHash<QString,QString> ServerData; //server,password
        QHash<QString,irc_channel*> ChanList;
        QHash<QString,private_message*> Queries;

    protected Q_SLOTS:
        void on_connected();
        void on_disconnected();
        void on_msgQuit(const QString& origin, const QString& message);
        void on_msgJoined(const QString& origin, const QString& channel);
        void on_msgNickChanged(const QString& origin, const QString& newNick);
        void on_msgCtcpReplyReceived(const QString& origin, const QString& reply);
        void on_msgCtcpRequestReceived(const QString& origin, const QString& request);
        void on_msgUnknownMessageReceived(const QString& origin, const QStringList& params);
        void on_msgParted(const QString& origin, const QString& channel, const QString& message);
        void on_msgInvited(const QString& origin, const QString& receiver, const QString& channel);
        void on_msgTopicChanged(const QString& origin, const QString& channel, const QString& topic);
        void on_msgNumericMessageReceived(const QString& origin, uint code, const QStringList& params);
        void on_msgNoticeReceived(const QString& origin, const QString& receiver, const QString& notice);
        void on_msgMessageReceived(const QString& origin, const QString& receiver, const QString& message);
        void on_msgCtcpActionReceived(const QString& origin, const QString& receiver, const QString& action);
        void on_msgKicked(const QString& origin, const QString& channel, const QString& nick, const QString& message);
        void on_msgModeChanged(const QString& origin, const QString& receiver, const QString& mode, const QString& args);

    signals:
        void ctcp_reply(IRC_Session*,Message);
        void ctcp_action(IRC_Session*,Message);
        void ctcp_request(IRC_Session*,Message);

        void event_connected(IRC_Session*);
        void event_disconnected(IRC_Session*); //Will come with disconnection data later
        void event_quit(IRC_Session*,Message);
        void event_nick(IRC_Session*,QString);
        void event_topic(IRC_Session*,QString);
        void even_notice(IRC_Session*,Message);
        void event_joined(IRC_Session*,QString);
        void event_newChannel(IRC_Session*,QString);
        void event_mode(IRC_Session*,QString,Message);
        void event_kick(IRC_Session*,QString,Message);
        void event_parted(IRC_Session*,QString,Message);
        void event_channel_invite(IRC_Session*,QString);
        void event_invite(IRC_Session*,QString,QString);
        void event_kicked(IRC_Session*,QString,Message);
        void event_unknown_notice(IRC_Session*,Message);
        void event_private_message(IRC_Session*,Message);
        void event_external_notice(IRC_Session*,Message);
        void event_parted_channel(IRC_Session*,QString,Message);
        void event_channel_message(IRC_Session*,QString,Message);
};

#endif // SESSION_H
