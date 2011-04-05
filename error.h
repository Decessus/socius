#ifndef ERROR_H
#define ERROR_H
#include <QString>
#include <QThread>
#include <QQueue>
#include <QHash>
#include <QTime>

namespace Error {
    enum flag {
        SocketError,NetworkError,InitError
    };

    enum sender {
        _IRC,_Twitter,_FaceBook,_MySpace,_Photobucket,_Picasa,
        _Flickr,_MediaStreamModule
    };

    enum state {
        STATE_ALERT,STATE_FATAL,STATE_SILENT
    };
};

struct ErrorData {
    QTime Time;
    signed long Code;
    QString Description;
    QHash<QString,QString> ExtraParams;

    Error::flag Flag;
    Error::state State;
    Error::sender Origin;
};

class ErrorManager : public QThread {
    Q_OBJECT
    virtual void run();

public:
    explicit ErrorManager(QObject *parent = 0);
    ~ErrorManager();

    void Watch(QObject *Sender,const char *Signal);

protected Q_SLOTS:
    void ReceiveError(ErrorData *error);
    void StopWatching(QObject *Sender,const char *Signal);

private:
    QQueue<ErrorData> Errors;

}ErrorReport;

#endif // ERROR_H
