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
    ~IRC();

private:
    Ui::IRC *ui;
    IRC_Session &activeSession;
    IRC_Settings *settings;
    QMap<QString,IRC_Session> activeSessions;
    QMap<QString,QString> command_aliases;
    QMap<QString,int(*function)(IRC_Session,irc_event_data)> commands;

private slots:


private slots:
    void on_messageField_returnPressed();
};

#endif // IRC_H
