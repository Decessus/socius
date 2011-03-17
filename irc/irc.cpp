#include "irc.h"
#include "ui_irc.h"

IRC::IRC(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::IRC)
{
    ui->setupUi(this);
}

IRC::~IRC()
{
    delete ui;
}
