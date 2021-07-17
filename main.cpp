#include "gui/informerwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    InformerWindow w;
    w.show();
    return a.exec();
}
