#include "tableWidgetDemo.h"
#include "pictureDisplayWidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TableWidgetDemo w;
    //PictureDisplayWidget w;

    w.show();
    return a.exec();
}
