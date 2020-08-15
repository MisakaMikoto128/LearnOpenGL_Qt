#include "GLWiget.h"
#include <QApplication>
#include "CoreFunctionWidget.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GLWiget w;
    w.resize(1000, 800);
    w.show();

    return a.exec();
}
