#include <QApplication>
#include "Client.h"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    Client window;
    window.setWindowTitle("nmaszin IRC");
    window.show();

    return application.exec();
}
