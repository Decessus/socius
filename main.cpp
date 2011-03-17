#include <QtGui/QApplication>
#include "socius.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    socius w;
    w.show();

    return a.exec();
}
