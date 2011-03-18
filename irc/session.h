#ifndef SESSION_H
#define SESSION_H

#include <QTextEdit>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>

#include "utilities.h"

class IRC_Session : public Irc::Session {
    Q_OBJECT

    public:
        IRC(QObject* parent = 0,QRect* frameSize = 0,QTreeWidget* sparent = 0);

        QMap<QString,irc_channel*> ChanList;
        QTreeWidgetItem *ServerItem;
        irc_channel *activeChannel;

        QTextEdit *chanText;
        QLineEdit *chanInput,*chanTitle;
        QListWidget *nickList;
        QFrame *sessionFrame;
        QPushButton *nickButton;

        QRect *frameSize;
        QString server,nick;
        QMap<QString,QString> *EventTemplates;

        bool isActive;

    protected Q_SLOTS:
        void on_connected();
        void on_disconnected();

        void on_msgJoined(const QString& origin, const QString& channel);
        void on_msgParted(const QString& origin, const QString& channel, const QString& message);
        void on_msgQuit(const QString& origin, const QString& message);
        void on_msgNickChanged(const QString& origin, const QString& nick);
        void on_msgModeChanged(const QString& origin, const QString& receiver, const QString& mode, const QString& args);
        void on_msgTopicChanged(const QString& origin, const QString& channel, const QString& topic);
        void on_msgInvited(const QString& origin, const QString& receiver, const QString& channel);
        void on_msgKicked(const QString& origin, const QString& channel, const QString& nick, const QString& message);
        void on_msgMessageReceived(const QString& origin, const QString& receiver, const QString& message);
        void on_msgNoticeReceived(const QString& origin, const QString& receiver, const QString& notice);
        void on_msgCtcpRequestReceived(const QString& origin, const QString& request);
        void on_msgCtcpReplyReceived(const QString& origin, const QString& reply);
        void on_msgCtcpActionReceived(const QString& origin, const QString& receiver, const QString& action);
        void on_msgNumericMessageReceived(const QString& origin, uint code, const QStringList& params);
        void on_msgUnknownMessageReceived(const QString& origin, const QStringList& params);

    public Q_SLOTS:
        void activateSession();
        void deactivateSession();
        void resize(QRect frameSize = 0);

    signals:
        void loadFinished();
        void on_msg(QString,QString);
        void event(IRC_Session,irc_event_data);
};

#endif // SESSION_H
