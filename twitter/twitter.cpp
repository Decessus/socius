/**
  During Alpha Stages
  **/

#include "twitter.h"
#include "error.h"

twitter::twitter(QObject *parent, QString user, QString pass) :
    QObject(parent)
{
    userName = user.toAscii(); password = pass.toAscii();
    autoAuthenticate = true;
}

void twitter::init() {
    qoauth = new QOAuth::Interface;
    qoauth->setConsumerKey( "2rKBmZOs1qQRW0a3jDY8g" );
    qoauth->setConsumerSecret( "XmXMFvNiuvoVVqpEL4HA02Ghxp7CqRRdAgCsjrjVI" );
    qoauth->setRequestTimeout( 10000 );
    webFrame->setText("Requesting Token");

    QOAuth::ParamMap reply = qoauth->requestToken( "https://api.twitter.com/oauth/request_token",
                                                   QOAuth::GET, QOAuth::HMAC_SHA1 );

    if(qoauth->error() == QOAuth::NoError) {
        webFrame->setText("Token Recieved");
        tokenKey = reply.value( QOAuth::tokenParameterName() );
        tokenSecret = reply.value( QOAuth::tokenSecretParameterName() );
        authorizeUser();
    }

    else {
        errorData *error = new errorData;
        switch(qoauth->error()) {

        case QOAuth::BadRequest :
            error->description = "Could not initialize due to bad request.";
            error->errorFlag = errorInfo::NetworkError;
            error->errorOrigin = errorInfo::_Twitter;
            emit emitError(error);
        break;

        case QOAuth::RSAKeyFileError :
            error->description = "Could not initialize: The provided RSA key file either doesn't exist or is unreadable.";
            error->errorFlag = errorInfo::NetworkError;
            error->errorOrigin = errorInfo::_Twitter;
            emit emitError(error);
        break;

        case QOAuth::RSADecodingError :
            webFrame->append("\nError: ");
            error->description = "Could not initialize: There was a problem decoding the RSA private key (the key is invalid or the provided passphrase is incorrect)";
            error->errorFlag = errorInfo::NetworkError;
            error->errorOrigin = errorInfo::_Twitter;
            emit emitError(error);
        break;

        case QOAuth::RSAPrivateKeyEmpty :
            webFrame->append("\nError: RSA private key has not been provided");
        break;

        case QOAuth::UnsupportedHttpMethod :
            webFrame->append("\nError: The HTTP method is not supported by the request");
        break;

        case QOAuth::ConsumerSecretEmpty :
            webFrame->append("\nError: Consumer secret has not been provided");
        break;

        case QOAuth::ConsumerKeyEmpty :
            webFrame->append("\nError: ConsumerKeyEmpty");
        break;

        case QOAuth::Timeout :
            webFrame->append("\nError: Reply from server timed out");
        break;

        case QOAuth::Forbidden :
            webFrame->append("\nError: 403 (Forbidden)");
        break;

        case QOAuth::Unauthorized :
            webFrame->append("\nError: 401 (Unauthorized)");
        break;

        default:
            webFrame->append("\nError: Unknown Error");
        }
    }
}

void twitter::authorizeUser() {
    QByteArray url( "http://api.twitter.com/oauth/authorize" );
    QOAuth::ParamMap map;
    map.insert("oauth_token",tokenKey);

    QByteArray header = qoauth->createParametersString( url, QOAuth::GET,tokenKey, tokenSecret,
                                                        QOAuth::HMAC_SHA1, map,
                                                        QOAuth::ParseForHeaderArguments );
    // append parameters string to the URL
    // alternatively you can use QOAuth::ParseForRequestContent if you want
    // to use the output as a POST request content (remember then of passing
    // QOAuth::POST above).

    authPageView = new QWebView(authRequestFrame);
    authPageView->setGeometry(0,0,739,441);
    authPageView->setVisible(true);
    authPageView->raise();

    QObject::connect(authPageView, SIGNAL(loadFinished(bool)),
                     this, SLOT(UrlUpdated(bool)));

    url.append( qoauth->inlineParameters( map, QOAuth::ParseForInlineQuery ) );
    webFrame->append("URL: " + url);
    authPageView->setUrl(QUrl(url));
    accountStatus = STATUS_Authenticating;
}

void twitter::UrlUpdated(bool finished) {

    switch(accountStatus) {
        case STATUS_Authenticating:

            webFrame->clear();
            webFrame->append("URL: " + authPageView->url().toString());
            webFrame->append("Page Data:\n\n " + authPageView->page()->mainFrame()->toHtml());
            QString htmlTemp = authPageView->page()->mainFrame()->toHtml();

            if(htmlTemp.indexOf("<div id=\"oauth_pin\">",0) != -1) {

                htmlTemp.replace(0,htmlTemp.indexOf("<div id=\"oauth_pin\">") + 20,"");
                htmlTemp.replace(htmlTemp.indexOf("</div>"),
                                 htmlTemp.size()-htmlTemp.indexOf("\""),"");

                while(htmlTemp.indexOf(" ",0) != -1) {
                    htmlTemp.replace(htmlTemp.indexOf(" "),1,"");
                }
                tokenVerifier = htmlTemp.toAscii();

                htmlTemp = authPageView->page()->mainFrame()->toHtml();

                while(htmlTemp.indexOf("\B<meta content=\"[*]\"name=\"verify-v1\">\B",0) != -1) {

                    htmlTemp.replace(0,htmlTemp.indexOf("\B<meta content=\"[*]\"name=\"verify-v1\">\B") + 15,"");
                    htmlTemp.replace(htmlTemp.indexOf("="),
                                     htmlTemp.size()-htmlTemp.indexOf("="),"");

                }

                while(htmlTemp.indexOf(" ",0) != -1) {
                    htmlTemp.replace(htmlTemp.indexOf(" "),1,"");
                }
                signature = htmlTemp.toAscii();

                extraParams.insert("oauth_verifier",tokenVerifier);
                extraParams.insert("oauth_token",tokenKey);
                extraParams.insert("oauth_consumer_key",qoauth->consumerKey());
                extraParams.insert("oauth_signature",signature);

                /*QByteArray header = qoauth->createParametersString( url, QOAuth::GET,tokenKey, tokenSecret,
                                                                    QOAuth::HMAC_SHA1, map,
                                                                    QOAuth::ParseForHeaderArguments );
*/

                reply =
                qoauth->accessToken( "https://api.twitter.com/oauth/access_token", QOAuth::POST, tokenKey,
                                     tokenSecret, QOAuth::HMAC_SHA1, extraParams );


                if ( qoauth->error() == QOAuth::NoError ) {
                  tokenKey = reply.value( QOAuth::tokenParameterName() );
                  tokenSecret = reply.value( QOAuth::tokenSecretParameterName() );
                  webFrame->append("Token Recieved: " + tokenKey + "\n" + tokenSecret +
                                   "\n" + signature + "\n" + tokenVerifier);
                }

                else {
                    errorData *error = new errorData;
                    switch(qoauth->error()) {

                    case QOAuth::BadRequest :
                        error->description = "Could not initialize due to bad request.";
                        error->errorFlag = errorInfo::NetworkError;
                        error->errorOrigin = errorInfo::_Twitter;
                        emit emitError(error);
                    break;

                    case QOAuth::RSAKeyFileError :
                        error->description = "Could not initialize: The provided RSA key file either doesn't exist or is unreadable.";
                        error->errorFlag = errorInfo::NetworkError;
                        error->errorOrigin = errorInfo::_Twitter;
                        emit emitError(error);
                    break;

                    case QOAuth::RSADecodingError :
                        webFrame->append("\nError: ");
                        error->description = "Could not initialize: There was a problem decoding the RSA private key (the key is invalid or the provided passphrase is incorrect)";
                        error->errorFlag = errorInfo::NetworkError;
                        error->errorOrigin = errorInfo::_Twitter;
                        emit emitError(error);
                    break;

                    case QOAuth::RSAPrivateKeyEmpty :
                        webFrame->append("\nError: RSA private key has not been provided");
                    break;

                    case QOAuth::UnsupportedHttpMethod :
                        webFrame->append("\nError: The HTTP method is not supported by the request");
                    break;

                    case QOAuth::ConsumerSecretEmpty :
                        webFrame->append("\nError: Consumer secret has not been provided");
                    break;

                    case QOAuth::ConsumerKeyEmpty :
                        webFrame->append("\nError: ConsumerKeyEmpty");
                    break;

                    case QOAuth::Timeout :
                        webFrame->append("\nError: Reply from server timed out");
                    break;

                    case QOAuth::Forbidden :
                        webFrame->append("\nError: 403 (Forbidden)");
                    break;

                    case QOAuth::Unauthorized :
                        webFrame->append("\nError: 401 (Unauthorized)");
                        webFrame->append("Token Recieved: " + tokenKey + "\n" + tokenSecret +
                                         "\n" + signature + "\n" + tokenVerifier);

                    break;

                    default:
                        webFrame->append("\nError: Unknown Error");
                    }
                }

            }
            break;
    }
}

void twitter::requestAccess(QString data) {

    QByteArray url( "https://api.twitter.com/oauth/authorize" );
    QOAuth::ParamMap map;
    QXmlStreamReader authPage;
    QByteArray authToken;

    authPage.addData(data.toStdString().c_str());

    while(!authPage.atEnd() && !authPage.hasError()) {

        QXmlStreamReader::TokenType token = authPage.readNext();

        if(token == QXmlStreamReader::StartDocument) {
                continue;
        }

        if(token == QXmlStreamReader::StartElement) {

            if(authPage.name() == "input") {

                QXmlStreamAttributes attrs = authPage.attributes();
                QStringRef attr = attrs.value("name");

                if(!attr.isEmpty()) {

                    if (attr.toString().toAscii() == "authenticity_token") {
                        attr = attrs.value("value");
                        if(!attr.isEmpty())
                            authToken = attr.toString().toAscii();
                    }

                    if (attr.toString().toAscii() == "oauth_token") {
                        attr = attrs.value("value");
                        if(!attr.isEmpty())
                            tokenKey = attr.toString().toAscii();
                    }
                }

                else {
                    authPage.readNext();
                    continue;
                }

            }

            else {
                authPage.readNext();
                continue;
            }

        }

    }

    if(authPage.hasError()) {

        errorData *error = new errorData;
        error->errorOrigin = errorInfo::_Twitter;
        error->errorFlag = errorInfo::RequestError;
        error->description = "Error parsing authentication data recieved from Twitter.\nError Recieved: "
                             + authPage.errorString() + "\n\nauthenticity_token=\"" + authToken + "\"";
        emit emitError(error);
        return;
    }

    authPage.clear();

    if(autoAuthenticate) {

        QByteArray postData;
        postData.append("oauth_token=" + tokenKey + "&");
        postData.append("authenticity_token=" + authToken + "&");
        postData.append("return_to_ssl=&");
        postData.append("redirect_after_login=/oauth/approved/Socius&");
        postData.append("session[username_or_email]=" + userName + "&");
        postData.append("session[password]=" + password + "&");
        postData.append("remember_me=1");

        networkAccess = new QNetworkAccessManager(this);
        QObject::connect(networkAccess, SIGNAL(finished(QNetworkReply*)),
                         this, SLOT(recieveAuth(QNetworkReply*)));

        webFrame->clear();
        webFrame->append("URL: " + url);

        networkAccess->post(QNetworkRequest(QUrl("https://api.twitter.com/sessions")),postData);

    }

    else {

    }

}

/*void twitter::recieveAuth(QNetworkReply *reply) {
    networkAccess->disconnect(SIGNAL(finished(QNetworkReply*)),
                        this, SLOT(recieveAuth(QNetworkReply*)));

    QVariant statusCodeV =
        reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    QUrl redirectionTargetUrl =
        reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();

    if(!redirectionTargetUrl.isEmpty()) {
        networkAccess = new QNetworkAccessManager(this);
        QObject::connect(networkAccess, SIGNAL(finished(QNetworkReply*)),
                         this, SLOT(recieveAuth(QNetworkReply*)));

        webFrame->append("URL: " + redirectionTargetUrl.toString());
        networkAccess->get(QNetworkRequest(redirectionTargetUrl));
        return;
    }

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray bytes = reply->readAll();  // bytes
        QString string(bytes); // string

        webFrame->append("ReplyData:\n" + string);
    }

    else {
    // handle errors here
    }
}

void twitter::recieveAccess(QNetworkReply *reply) {

    networkAccess->disconnect(SIGNAL(finished(QNetworkReply*)),
                        this, SLOT(recieveAccess(QNetworkReply*)));

    QVariant statusCodeV =
        reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    QVariant redirectionTargetUrl =
        reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray bytes = reply->readAll();  // bytes
        QString string(bytes); // string
        webFrame->append("ReplyData:\n" + string);
    }

    else {
    // handle errors here
    }

}*/

void twitter::networkReply(QNetworkReply *reply) {

    networkAccess->disconnect(SIGNAL(finished(QNetworkReply*)),
                        this, SLOT(networkReply(QNetworkReply*)));

    QVariant statusCodeV =
        reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    QVariant redirectionTargetUrl =
        reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

    if (reply->error() == QNetworkReply::NoError) {

        QByteArray bytes = reply->readAll();  // bytes
        QString string(bytes); // string
        webFrame->append("ReplyData:\n" + string);
        requestAccess(string);
    }

    else {
    // handle errors here
    }

}

void twitter::tweet(QString message) {
      QOAuth::ParamMap otherArgs;
/*
      reply =
      qoauth->accessToken( "https://api.twitter.com/oauth/access_token", QOAuth::POST, token,
                           tokenSecret, QOAuth::HMAC_SHA1, otherArgs );

      if ( qoauth->error() == QOAuth::NoError ) {
        token = reply.value( QOAuth::tokenParameterName() );
        tokenSecret = reply.value( QOAuth::tokenSecretParameterName() );
        webFrame->setText("Token Recieved: " + token + "\n" + tokenSecret);
      } */

}
