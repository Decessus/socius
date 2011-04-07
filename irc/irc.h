#ifndef IRC_H
#define IRC_H

#include <QFrame>
#include <QColor>

#include "session.h"
#include "utilities.h"

namespace Ui {
    class IRC;
}

class IRC : public QFrame
{
    Q_OBJECT

public:
    explicit IRC(QWidget *parent = 0);

    QTextEdit *chanText;
    QLineEdit *chanInput,*chanTitle;
    QListWidget *nickList;
    QPushButton *nickButton;

    ~IRC();

signals:
    Q_INVOKABLE void onMSG(const IRC_Session*,const QString,const QString);

public Q_SLOTS:

    void ctcp_reply(IRC_Session* Session,Message Data);
    void ctcp_action(IRC_Session* Session,Message Data);
    void ctcp_request(IRC_Session* Session,Message Data);

    void event_connected(IRC_Session* Session);
    void event_disconnected(IRC_Session* Session); //Will come with disconnection data later
    void event_quit(IRC_Session* Session,Message Data);
    void event_nick(IRC_Session* Session,QString Channel);
    void event_topic(IRC_Session* Session,QString Channel);
    void even_notice(IRC_Session* Session,Message Data);
    void event_joined(IRC_Session* Session,QString Channel);
    void event_newChannel(IRC_Session* Session,QString Channel);
    void event_mode(IRC_Session* Session,QString Channel,Message Data);
    void event_kick(IRC_Session* Session,QString Channel,Message Data);
    void event_parted(IRC_Session* Session,QString Channel,Message Data);
    void event_invite(IRC_Session* Session,QString Nick,QString Channel);
    void event_kicked(IRC_Session* Session,QString Channel,Message Data);
    void event_unknown_notice(IRC_Session* Session,Message Data);
    void event_private_message(IRC_Session* Session,Message Data);
    void event_external_notice(IRC_Session* Session,Message Data);
    void event_parted_channel(IRC_Session* Session,QString Channel,Message Data);
    void event_channel_message(IRC_Session* Session,QString Channel,Message Data);
    void event_channel_invite(IRC_Session *Session,QString Nick,QString Channel,QString nChannel);

    void newSession();
    void newSession(QString host);
    void newSession(QString host,int port);
    void newSession(QString host, int port, QStringList args);
    void switchChannels(QString channel);

    void resize(QRect frameSize = QRect(0,0,0,0));
    void itemClicked(QTreeWidgetItem *item,int i);
    void onMsgEntered();

private:
    Ui::IRC *ui;

    bool wasStatus;

    void RefreshChannelUsers(IRC_Session *Session,QString Channel);
    void RefreshChannelTopic(IRC_Session *Session,QString Channel);
    void RefreshChannelMessages(IRC_Session *Session,QString Channel);

    QString ThemeMessage(Message message);

    QList<IRC_Session*> sessions;
    QHash<QString,QString> EventTemplates;

    struct EventColors {
        QColor Mode,Quit,Topic,Kicked,
            Parted,Joined,Nick,Notice,Invite,
            Message,Connected,Mentioned,
            Disconnected,ChannelInvite;
    };

    EventColors eventColor;
private slots:
};

#endif // IRC_H
