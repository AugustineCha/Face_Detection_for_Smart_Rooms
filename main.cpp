#include "smartrooms.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SmartRooms w;
    w.show();
    return a.exec();
}
