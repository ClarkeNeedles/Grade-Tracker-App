#include "../include/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow mainPage;

    mainPage.show();

    return app.exec();
}
