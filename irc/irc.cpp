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

    connect(chanInput,SIGNAL(returnPressed()),this,SLOT(onMsgEntered()));

    chanText = new QTextEdit(this);
    chanText->setGeometry(0,22,width()-145,height()-44);
    chanText->setVisible(true);

    chanInput = new QLineEdit(this);
    chanInput->setGeometry(102,height()-22,22,width()-102);
    chanInput->setVisible(true);

    nickButton = new QPushButton(this);
    nickButton->setGeometry(0,height()-22,22,102);
    nickButton->setVisible(true);

    nickList = new QListWidget(this);
    nickList->setGeometry(width()-145,22,height()-44,145);
    nickList->setVisible(true);

    chanTitle = new QLineEdit(this);
    chanTitle->setGeometry(0,0,22,width());
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
    if(item->childCount()>0 || item->text(0) == "< new >") {

        int i;
        for(i=0;i<sessions.count();i++) {
            if(sessions.at(i)->IsActive)
                sessions.at(i)->IsActive = false;

            if(sessions.at(i)->ServerItem == item)
                break;
        }

        //Server was found, otherwise there was an OOB Error occurrence
        if(i<sessions.count()) {
            sessions.at(i)->IsActive = true;
            RefreshChannelMessages(sessions.at(i),"status");
            //Change the GUI for Status Window
            chanTitle->setVisible(false);
            nickList->setVisible(false);
        }
    }

    else {

        //Recover from status window
        if(!chanTitle->isVisible())
            chanTitle->setVisible(true);

        if(!nickList->isVisible())
            nickList->setVisible(true);

        int i;
        for(i=0;i<sessions.count();i++) {
            if(sessions.at(i)->IsActive)
                sessions.at(i)->IsActive = false;

            if(sessions.at(i)->ServerItem == item->parent())
                break;
        }

        //Server was found, otherwise there was an OOB Error occurrence
        if(i<sessions.count()) {
            sessions.at(i)->IsActive = true;
            RefreshChannelMessages(sessions.at(i),item->text(0));
            RefreshChannelTopic(sessions.at(i),item->text(0));
            RefreshChannelUsers(sessions.at(i),item->text(0));
        }
    }
}

void IRC::RefreshChannelMessages(IRC_Session *Session, QString Channel) {
    chanText->clear();
    QQueue<Message> temp = Session->ChanList[Channel]->getMessages();

    while (!temp.isEmpty())
        chanText->append(ThemeMessage(temp.dequeue()));
}

void IRC::RefreshChannelTopic(IRC_Session *Session, QString Channel) {
    chanTitle->setText(Session->ChanList[Channel]->topic);
}

void IRC::RefreshChannelUsers(IRC_Session *Session, QString Channel) {
    QStringList temp = Session->ChanList[Channel]->users;

    //Initially sort by channel mode, then alphabetically
    QStringList Owner,Sop,Op,Hop,Voice,NoMode;

    foreach (const QString& user,temp) {

        switch (user.at(0)) {

        case '~':
            Owner.append(user);
            break;

        case '&':
            Sop.append(user);
            break;

        case '@':
            Op.append(user);
            break;

        case '%':
            Hop.append(user);
            break;

        case '+':
            Voice.append(user);
            break;

        default:
            NoMode.append(user);
            break;
        }

    }

    //Delete the physical QListWidgetItems from the nick list
    for(int i=0;i<nickList->count();i++)
        delete nickList->item(i);

    //Add and sort owners
    Owner.sort();
    foreach (const QString& user,Owner)
        nickList->addItem(new QListWidgetItem(user));

    //Add and sort owners
    Sop.sort();
    foreach (const QString& user,Sop)
        nickList->addItem(new QListWidgetItem(user));

    //Add and sort owners
    Op.sort();
    foreach (const QString& user,Op)
        nickList->addItem(new QListWidgetItem(user));

    //Add and sort owners
    Hop.sort();
    foreach (const QString& user,Hop)
        nickList->addItem(new QListWidgetItem(user));

    //Add and sort owners
    Voice.sort();
    foreach (const QString& user,Voice)
        nickList->addItem(new QListWidgetItem(user));

    //Add and sort owners
    NoMode.sort();
    foreach (const QString& user,NoMode)
        nickList->addItem(new QListWidgetItem(user));

    return;
}
