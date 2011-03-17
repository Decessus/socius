#ifndef TWITTER_H
#define TWITTER_H

#include <QObject>
#include <QtOAuth>
#include <QByteArray>
#include <QtNetwork>
#include <QtWebKit/QtWebKit>
#include <QFrame>
#include <QTextEdit>
#include <QLabel>
#include <QTimer>
#include <QtXml/QXmlStreamReader>

#include "updateEvent.h"
#include "socialsource.h"
#include "error.h"

class twitter : public QObject
{
    enum STATUS {
        STATUS_Authenticating,STATUS_Authenticated,STATUS_AuthenticationFailure,
        STATUS_RecievedRequest,STATUS_RecievedAccess,STATUS_RequestingToken,
        STATUS_RequestingAccess
    }accountStatus;

    Q_OBJECT
public:
    explicit twitter(QObject *parent,QString user,QString pass);

    void updateIntervalRate(int rate);
    void accountDetails(QString username,QString password);
    void tweet(QString message);

    void init();

    QTextEdit *webFrame;
    QWidget *authRequestFrame;

signals:
    void updated(QString,QString,updateEvent);
    void emitError(errorData*);
    void status(QString,QString);

public slots:
    void networkReply(QNetworkReply* reply);
    void UrlUpdated(bool finished);
private:
    QOAuth::ParamMap extraParams;
    QOAuth::ParamMap reply;
    QByteArray tokenKey,tokenSecret,userName,password,tokenVerifier,signature;
    QOAuth::Interface *qoauth;
    QNetworkAccessManager *networkAccess;

    QTimer updateTimer;
    QWebView *authPageView;
    bool autoAuthenticate;

    void requestAccess(QString data);
    void authorizeUser();
};

#endif // TWITTER_H
