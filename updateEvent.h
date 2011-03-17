#ifndef UPDATEEVENT_H
#define UPDATEEVENT_H
#include <QString>
#include <QStringList>
#include <QTime>

struct updateEvent {


    QString user,status;
    QTime time;
    QStringList pictures;

    bool newProfilePicture;
    bool newPhotoUploads;
    bool newStatus;
};

#endif // UPDATEEVENT_H
