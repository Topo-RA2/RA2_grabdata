#include "ra2mem.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
        RA2Mem w;
        w.show();
    return a.exec();
}
