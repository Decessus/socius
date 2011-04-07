#include "session.h"

void IRC_Session::on_msgNumericMessageReceived(const QString& origin, uint code, const QStringList& params) {
    switch(code) {

        /* Topic change already handled by libircclient-qt, was this a possible fix?
    case 332:

        if(ChanList[origin]) {
            ChanList[origin]->topic = params.at(2);

            if((activeChannel->chanId->text() == origin) && isActive)
                chanTitle->setText(params.at(2));

        }

    break;
    */

    case 353:
        if(ChanList[params.at(2)]) {

            QString buff = params.at(3);
            ChanList[params.at(2)]->users = buff.split(" ",QString::SkipEmptyParts);

            if((activeChannel->chanId->text() == origin) && isActive) {
                nickList->clear();
                for (int i=0;i<this->ChanList[params.at(2)]->users.size();++i) {
                    nickList->addItem(new QListWidgetItem(ChanList[params.at(2)]->users.at(i)));
                }
            }

        }
    break;

    default:

    }

}
