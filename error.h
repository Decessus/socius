#ifndef ERROR_H
#define ERROR_H
#include <QString>
#include <QTime>

namespace errorInfo {
    enum flag {
        HTTPError,NetworkError,RequestError,RequestUnauthorized,
        RequestUnknown,RequestTimedOut,OAuthError,TransferError,
        ScriptError,ScriptLogicalError,ThemeError
    };

    enum sender {
        _IRC,_Twitter,_FaceBook,_MySpace,_Photobucket,_Picasa,
        _Flickr,_MediaStreamModule
    };
};

struct errorData {
    QTime timeStamp;
    QString errorCode,description;
    errorInfo::flag errorFlag;
    errorInfo::sender errorOrigin;
};

#endif // ERROR_H
