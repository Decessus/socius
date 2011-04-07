#ifndef SESSION_H
#define SESSION_H

#include <QTextEdit>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QPixmap>
#include <QHash>

#include "channel.h"
#include "utilities.h"

class IRC_Session : public Irc::Session {
    Q_OBJECT

    enum OperStatus {
        NoOper,HelpOper,LocalOper,GlobalOper,
        TechnicalAdmin,NetworkAdmin,ServerAdmin
    };

    public:
        friend class IRC;
        ~IRC_Session();
        explicit IRC_Session(QObject* parent = 0,QTreeWidget* sParent = 0);
        explicit IRC_Session(ServerProfile* Profile = 0,QObject* parent = 0,QTreeWidget* sParent = 0);

        bool IsActive;

        QString ActiveChannel;
        QHash<QString,irc_channel*> ChanList;

        QTreeWidgetItem *ServerItem;

        /** Active messages
          Messages that will appear in the first channel that activated
          within the session.
          **/
        QQueue<Message> ActiveMessages;
        QHash<QString,User> KnownUsers;

    private:
        QByteArray Modes;
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

        ServerProfile *Profile;

        QHash<QString,private_message*> Queries;
        QHash<QString,QPixmap> UserDisplayPicCache;

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
        void event_invite(IRC_Session*,QString,QString);
        void event_kicked(IRC_Session*,QString,Message);
        void event_unknown_notice(IRC_Session*,Message);
        void event_private_message(IRC_Session*,Message);
        void event_external_notice(IRC_Session*,Message);
        void event_parted_channel(IRC_Session*,QString,Message);
        void event_channel_message(IRC_Session*,QString,Message);
        void event_channel_invite(IRC_Session*,QString,QString,QString);
};

#endif // SESSION_H
