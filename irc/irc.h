#ifndef IRC_H
#define IRC_H

#include <QFrame>

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
    Q_INVOKABLE void onMSG(IRC_Session*,QString,QString);

public Q_SLOTS:

    void onMessage(IRC_Session* session,irc_channel channel);
    void onJoined(IRC_Session* session,irc_channel channel);
    void onParted(IRC_Session* session,irc_channel channel);
    void onQuit(IRC_Session* session,irc_channel channel);
    void onNickChanged(IRC_Session* session,irc_channel channel);
    void onModeChanged(IRC_Session* session,irc_channel channel);
    void onTopicChanged(IRC_Session* session,irc_channel channel);
    void onInvited(IRC_Session* session,irc_channel channel);
    void onKicked(IRC_Session* session,irc_channel channel);
    void onMessageReceived(IRC_Session* session,irc_channel channel);
    void onNoticeReceived(IRC_Session* session,irc_channel channel);
    void onCtcpRequestReceived(IRC_Session* session,irc_channel channel);
    void onCtcpReplyReceived(IRC_Session* session,irc_channel channel);
    void onCtcpActionReceived(IRC_Session* session,irc_channel channel);
    void onNumericMessageReceived(IRC_Session* session,irc_channel channel);
    void onUnknownMessageReceived(IRC_Session* session,irc_channel channel);

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
    IRC_Session *activeSession;

    bool wasStatus;

    QMap<QString,IRC_Session*> sessions;

private slots:
};

#endif // IRC_H
