#include "socius.h"
#include "ui_socius.h"
#include "error.h"

socius::socius(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::socius)
{
    ui->setupUi(this);
    twitter *twitt = new twitter(this,"email","password");
    twitt->webFrame = ui->textEdit;
    twitt->authRequestFrame = ui->authWidget;
    connect(twitt,SIGNAL(emitError(errorData*)),this,SLOT(getError(errorData*)));
    twitt->init();
}
void socius::getError(errorData* error) {
    QString errorTitle;

    switch(error->errorOrigin) {
    case errorInfo::_Twitter:
        errorTitle = "Socius Social Engine Error From Twitter";
        break;
    }

    QMessageBox::critical(this,
                          errorTitle,
                          error->description,
                          QMessageBox::Ok);
}

socius::~socius()
{
    delete ui;
}
void socius::loadSettings(QString file) {
/*
    if(file) {
        QFile* sfile = new QFile(file);
        QDomDocument* settingsRoot = new QDomDocument("Settings");

        if (!sfile->open(QIODevice::ReadOnly | QIODevice::Text)) {
            if(sfile)
                delete sfile;

            if(settings)
                delete settings;

            QMessageBox::critical(this,
                                  "socius Settings Error",
                                  "Couldn't open " + file,
                                  QMessageBox::Ok);

            emit error("Error: File could not be opened..\nFile location: " + file);
            return;
        }


        if(!settingsRoot->setContent(&sfile)) {

            if(sfile)
                delete sfile;

            if(settings)
                delete settings;

            QMessageBox::critical(this,
                                  "socius Settings Error",
                                  "Couldn't stream content from " + file,
                                  QMessageBox::Ok);

            emit error("Error: Couldn't generate stream content..\nFile location: " + file);
            return;
        }

        QDomElement* settElement = new QDomElement(settings->documentElement());

        if(settElement->isNull()) {
            if(sfile)
                delete sfile;

            if(settings)
                delete settings;

            if(settElement)
                delete settElement;

            QMessageBox::critical(this,
                                  "socius Settings Error",
                                  "Couldn't stream content from root element. Is the XML Valid?",
                                  QMessageBox::Ok);

            emit error("Error: Couldn't generate content from root element..\nFile location: " + file);
            return;
        }

        QDomNode* settNode = new QDomNode(settElement->firstChild());

        if(settNode->isNull()) {
            if(sfile)
                delete sfile;

            if(settings)
                delete settings;

            if(settElement)
                delete settElement;

            if(settNode)
                delete settNode;

            QMessageBox::critical(this,
                                  "socius Settings Error",
                                  "Content does not contain a node in root element. Is the XML Valid?",
                                  QMessageBox::Ok);

            emit error("Error: Content does not contain a node in the root element..\nFile location: " + file);
            return;
        }

        QDomElement nNode;
        while(!settNode->isNull()) {
            nNode = settNode->toElement();

            if(nNode.tagName() == QString::toLower("soundprofile")){

            }

            settingsMap[nNode.tagName()] = nNode;
            settNode = settNode->nextSibling();
        }

    } else {
        if(sfile)
            delete sfile;

        if(settings)
            delete settings;

        QMessageBox::critical(this,
                              "socius Settings Error",
                              "Couldn't open NULL File",
                              QMessageBox::Ok);
    } */

}

void socius::openTheme(QString filename)
{ /*
    QFile stylef(filename);

    if( !stylef.exists() )
    {
      // It does not exist
      ui->chantext->append("Style Sheet Does Not Exist..." + filename);

      return;
    }

    if( !stylef.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
      // It could not open
      ui->chantext->append("Style Sheet Failed To Open...");

      return;
    }

    QTextStream stream ( &stylef );
    QString nstyle;
    while( !stream.atEnd()) {
         nstyle.append(stream.readLine());
    }

    this->setStyleSheet(nstyle); */
}

void socius::on_activityStream_clicked()
{
    ui->authWidget->setVisible(true);
    if(!ui->activityStream->isChecked()) {
        ui->activityStream->setChecked(true);
        return;
    }

    if(ui->chatPortal->isChecked()) {
        ui->chatPortal->setChecked(false);
        return;
    }

    if(ui->myCloud->isChecked()) {
        ui->myCloud->setChecked(false);
        return;
    }

    if(ui->mediaPortal->isChecked()) {
        ui->mediaPortal->setChecked(false);
        return;
    }

    if(ui->scriptsPortal->isChecked()) {
        ui->webFrame->setVisible(false);
        ui->scriptsPortal->setChecked(false);
        return;
    }
}

void socius::on_chatPortal_clicked()
{
    if(!ui->chatPortal->isChecked()) {
        ui->chatPortal->setChecked(true);
        return;
    }

    if(ui->activityStream->isChecked()) {
        ui->activityStream->setChecked(false);
        return;
    }

    if(ui->myCloud->isChecked()) {
        ui->myCloud->setChecked(false);
        return;
    }

    if(ui->mediaPortal->isChecked()) {
        ui->mediaPortal->setChecked(false);
        return;
    }

    if(ui->scriptsPortal->isChecked()) {
        ui->scriptsPortal->setChecked(false);
        return;
    }
}

void socius::on_myCloud_clicked()
{
    if(!ui->myCloud->isChecked()) {
        ui->myCloud->setChecked(true);
        return;
    }

    if(ui->activityStream->isChecked()) {
        ui->activityStream->setChecked(false);
        return;
    }

    if(ui->chatPortal->isChecked()) {
        ui->chatPortal->setChecked(false);
        return;
    }

    if(ui->mediaPortal->isChecked()) {
        ui->mediaPortal->setChecked(false);
        return;
    }

    if(ui->scriptsPortal->isChecked()) {
        ui->scriptsPortal->setChecked(false);
        return;
    }
}

void socius::on_mediaPortal_clicked()
{
    if(!ui->mediaPortal->isChecked()) {
        ui->mediaPortal->setChecked(true);
        return;
    }

    if(ui->activityStream->isChecked()) {
        ui->activityStream->setChecked(false);
        return;
    }

    if(ui->myCloud->isChecked()) {
        ui->myCloud->setChecked(false);
        return;
    }

    if(ui->chatPortal->isChecked()) {
        ui->chatPortal->setChecked(false);
        return;
    }

    if(ui->scriptsPortal->isChecked()) {
        ui->scriptsPortal->setChecked(false);
        return;
    }
}

void socius::on_scriptsPortal_clicked()
{
    ui->webFrame->setVisible(true);
    if(!ui->scriptsPortal->isChecked()) {
        ui->scriptsPortal->setChecked(true);
        return;
    }

    if(ui->activityStream->isChecked()) {
        ui->activityStream->setChecked(false);
        ui->authWidget->setVisible(false);
        return;
    }

    if(ui->myCloud->isChecked()) {
        ui->myCloud->setChecked(false);
        return;
    }

    if(ui->mediaPortal->isChecked()) {
        ui->mediaPortal->setChecked(false);
        return;
    }

    if(ui->chatPortal->isChecked()) {
        ui->chatPortal->setChecked(false);
        return;
    }
}
