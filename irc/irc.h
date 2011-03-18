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

public slots:
    void joinChannel(IRC_Session session,QString channel);
    void partChannel(IRC_Session session,QString channel);

private slots:
};

#endif // IRC_H
