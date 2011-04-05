#ifndef UTILITIES_H
#define UTILITIES_H

#include <QStringList>
#include <QTreeWidgetItem>
#include <QTime>
#include <QList>
#include <QQueue>
#include <QMap>

#include "3rdParty/libircclient-qt-0.5.0/include/irc.h"
#include "3rdParty/libircclient-qt-0.5.0/include/ircbuffer.h"
#include "3rdParty/libircclient-qt-0.5.0/include/ircsession.h"
#include "3rdParty/libircclient-qt-0.5.0/include/ircutil.h"

/** The 'QSleep' function was taken from a snippet provided by http://berenger.eu/blog/ **/
class QSleep : public QThread {
     Q_OBJECT
     void run (){}

     public :
     static void usleep(long iSleepTime){
          QThread::usleep(iSleepTime);
     }
     static void sleep(long iSleepTime){
          QThread::sleep(iSleepTime);
     }
     static void msleep(long iSleepTime){
          QThread::msleep(iSleepTime);
     }
};

namespace Ui {
    class IRC;
}

struct irc_event_data {
    QString message, origin, target;
    QMap<QString,QString> otherData;
};

struct irc_server {
    QStringList addresses, autoJoinChannels, connectCommands;
    QString defaultAddress, nickPass, serverPass, networkName;
    int port;
    bool useSSL;
    bool autoConnect;
    bool bypassProxySettings;
};

struct stream_link {
    QString hostRoot;
    QRegExp extractMethod;
    const QString linkData;
};

struct IRC_Settings {
    QList<irc_server> serversList;
    QString proxyHost,proxyHostType;
    QMap<QString /*event*/,QString /*template*/> messageTemplates;
    int proxyHostPort;
};

#endif // UTILITIES_H
