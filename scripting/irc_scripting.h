#ifndef IRC_SCRIPTING_H
#define IRC_SCRIPTING_H

#include <QObject>
#include "../irc.h"

class sIRC : public QObject
{
    Q_OBJECT
public:
    explicit sIRC(QObject *parent = 0);
    Q_INVOKABLE virtual bool newSession(QString host);
    Q_INVOKABLE virtual bool sendMessage(QString target,QString message);
    Q_INVOKABLE virtual bool join(QString channel);
    Q_INVOKABLE virtual bool part(QString channel);
    Q_INVOKABLE virtual bool echo(QString target,QString message);
    Q_INVOKABLE virtual bool raw(QString message);
    Q_INVOKABLE virtual bool raw(QString target,QString message);
    Q_INVOKABLE virtual bool ctcp(QString target,QString data);

    /*
        STSP (Socius to Socius Protocol)
        --Allows users to use the Client to Client protocol features without
            having to be on the same server, as long as both users are logged in
            to the Socius Network.

        This feature will be made available once the Socius Network is online.
    */

    //Q_INVOKABLE virtual bool stsp(QString user,QString data);
signals:

public slots:

private:
    IRC_Session session;

};

#endif // IRC_SCRIPTING_H
