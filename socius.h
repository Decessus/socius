#ifndef SOCIUS_H
#define SOCIUS_H

#include <QMainWindow>
#include <QAction>
#include <qdom.h>
#include <QFile>
#include <QMessageBox>

#include "twitter/twitter.h"
#include "error.h"
#include "irc/irc.h"

namespace Ui {
    class socius;
}

class socius : public QMainWindow
{
    Q_OBJECT

public:
    explicit socius(QWidget *parent = 0);
    ~socius();

    void loadSettings(QString file);
    void openTheme(QString filename);

private slots:
    void on_scriptsPortal_clicked();
    void on_myCloud_clicked();
    void on_mediaPortal_clicked();
    void on_chatPortal_clicked();
    void on_activityStream_clicked();
    void getError(errorData* error);

private:
    Ui::socius *ui;

    IRC *ChatPortal;
};

#endif // SOCIUS_H
