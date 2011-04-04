/**
  Overhauling GUI/Session Relations.
  -Session to handle all incomming data, emitting events to the GUI handler
  **/

#include "irc.h"
#include "ui_irc.h"

IRC::IRC(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::IRC)
{
    ui->setupUi(this);

    setGeometry(0,
                55,
                parent->frameGeometry().width(),
                parent->frameGeometry().height()-55);

    sessionCount = 0;

    connect(chanInput,SIGNAL(returnPressed()),this,SLOT(onMsgEntered()));

    chanText = new QTextEdit(this);
    chanText->setGeometry(0,22,this->width()-145,this->height()-44);
    chanText->setVisible(true);

    chanInput = new QLineEdit(this);
    chanInput->setGeometry(102,this->height()-22,22,this->width()-102);
    chanInput->setVisible(true);

    nickButton = new QPushButton(this);
    nickButton->setGeometry(0,this->height()-22,22,102);
    nickButton->setVisible(true);

    nickList = new QListWidget(this);
    nickList->setGeometry(this->width()-145,22,this->height()-44,145);
    nickList->setVisible(true);

    chanTitle = new QLineEdit(this);
    chanTitle->setGeometry(0,0,22,this->width());
    chanTitle->setVisible(true);

}

IRC::~IRC()
{
    delete ui;
}

void IRC::newSession() {
    IRC_Session nSession(this,ui->serverTree);
    sessions.insert(nSession);
    activeSession = nSession;
    return;
}

void IRC::onMSG(IRC_Session *, QString, QString) {

}

void IRC::newSession(QString host) {

    sessionCount++;
    sessions[sessionCount] = new IRC_Session(this,
                                             QRect(1,
                                                   137,
                                                   frameGeometry().width()-137,
                                                   frameGeometry().height()-2),
                                             ui->serverTree);

}

void IRC::newSession(QString host, int port, QStringList args) {

}


/**
  GUI Slots
  **/

//Switch Channel GUI
void IRC::itemClicked(QTreeWidgetItem *item, int i) {

    //Was the item a session status window?
    if(sessions.contains(item)) {

        if(sessions[item] != activeSession)
           activeSession = sessions[item];

        //Change the GUI for Status Window
        chanTitle->setVisible(false);
        nickList->setVisible(false);
    }

    else if(sessions.contains(item->parent())) {
        if(sessions[item->parent()] != activeSession)
            activeSession = sessions[item->parent()];

        if(activeSession->ChanList.contains(item->text(0))) {
            activeSession->activeChannel = item->text(0);

            chanTitle->setText(activeSession->ChanList[channel]->topic);
            chanText->setText(activeSession->ChanList[channel]->text);

            //Delete the physical QListWidgetItems from the nick list
            for(int i=0;i<nickList->count();i++)
                delete nickList->item(i);

            //Add the new QListWidgetItems
            for(int i=0;i<activeSession->ChanList[channel]->users.size();i++)
                nickList->addItem(new QListWidgetItem(activeSession->ChanList[channel]->users.at(i)));
        }
    }
}

void IRC::disconnected(IRC_Session* session)
{

    for(QMap<QString,irc_channel*>::iterator i = session->ChanList.begin();
    i!=session->ChanList.end();i++) {
        i.chanId->setText("(" + i.chanId->text() + ")");
        i.text += ("Disconnected from " + host() + "..\n");
    }

    if(session == activeSession)
        chanText->setText(ChanList[activeChannel]->text);

    else {

        for(QList<IRC_Session>::iterator i = sessions.begin();i!=sessions.end();i++) {
            if(i.value() == session) {

            }
        }

    }

    return;

}
