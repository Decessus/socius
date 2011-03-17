#ifndef SOCIALSOURCE_H
#define SOCIALSOURCE_H

#include <QObject>
#include <QtOAuth>
#include <QtNetwork>
#include <QRegExp>

#include <error.h>
/*
struct OAuthSource {
    QOAuth::ParamMap extraParams;
    QOAuth::ParamMap reply;
    QByteArray token;
    QByteArray tokenSecret;
    QOAuth::Interface *qoauth;
    errorData *error;
};

struct HTTPSource {
    QByteArray url;
    QNetworkReply replyData;
    QNetworkRequest requestData;
    QNetworkAccessManager *sourceManager;
    errorData *error;
};

struct FTPSource {
    QByteArray url;
    QNetworkReply replyData;
    QNetworkRequest requestData;
    QNetworkAccessManager *sourceManager;
    errorData *error;
};

struct RawSource {
    QByteArray host;
    QNetworkReply replyData;
    QNetworkRequest requestData;
    QNetworkAccessManager *sourceManager;
    errorData *error;
}; */

class socialSource : public QObject
{
    enum sourceType {
        _OAUTH,_HTTP,_FTP,_SSH,_SSL,_RAW
    } sourceProtocol;

    Q_OBJECT
public:
    explicit socialSource(QObject *parent = 0);
    //void setAccountData(QString userName,QString password);
    //void setSourceProtocol(sourceType protocol);
signals:
    void recieved();
public slots:

private:


};

#endif // SOCIALSOURCE_H
