#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Create an instance of the main window
    MainWindow window;

    // Display that instance to the user
    window.show();

    // Event loop that allows the program to run until closed
    return a.exec();
}
