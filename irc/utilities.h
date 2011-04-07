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

namespace Status {

    /**
        Block: Block *all* private messages that you do not start
        Alert: Request user to allow new private message
        Known: Only accept private messages if you are in the channel with the user
        Friends: Only accept private messages from people you mark as a friend

            'Known' is the default state.
      **/
    enum PrivateMessage {
        PM_Alert,PM_Block,PM_Friends,PM_Known
    };

    /**
        Block: Block *all* channel invites
        Alert: Request user to join channel
        Idle: Show request in active channel
        Join: Auto join channel

            'Alert' is the default state.
      **/
    enum ChannelInvite {
        CI_Alert,CI_Block,CI_Idle,CI_Join
    };

    /**
        Auto: Automatically log by date
        Buffer: Only log the messages in the active buffer
        None: Never log
        Session: Per session end, ask if the users wants to log or not

            'Auto' is the default state
      **/
    enum LogState {
        LOG_Auto,LOG_Session,LOG_Buffer,LOG_None
    };

    /** User activity status **/
    enum UserStatus {
        Away,Busy,DoNotDisturb,Online
    };
}

struct ProfileInfo {
    QString Bio,
            Motto,
            Status,
            PersonalLink1,
            PersonalLink2,
            PersonalLink3;
};

struct UserProfile {

    bool AutoRejoinOnKick;

    QHash<QString,QString> NickNames;

    /** Global server and network settings **/
    QString Ident,
            RealName,
            ProxyUser,
            ProxyServer,
            ProxyPassword;

    /** Personal Profile Settings **/
    QPixmap *DisplayPicture;
    QStringList QuitMessages;

    ProfileInfo ProfileDetails;
    Status::LogState LoggingRule;
    Status::UserStatus OnlineStatus;
    Status::ChannelInvite ChannelInviteRule;
    Status::PrivateMessage PrivateMessageRule;
};

struct ServerProfile {

    quint16 DefaultPort,
            SSLPort;

    bool ForceProxyBypass,
            DetermineBestHost;

    QString ServerPassword,
            HostName;

    QStringList ConnectCommands,
            AutoJoinChannels,
            Servers;
};

struct User {
    bool IsIrcOper;

    QString RealName,
            UserName,
            AwayReason,
            HostServer,
            UserAddress,
            UserClientInfo,
            UserSignOnTime,
            HostServerMotto;

    QPixmap DisplayPic;
    QStringList Channels;
    QTime LastRefreshTime;
};

struct Message {
    QString eventType,sender,text,timestamp;
    QStringList extraParams;
};

const UserProfile *ClientProfile;

#endif // UTILITIES_H
