#include <QApplication>
#include "MainWindow.h"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv); //creating a Qt application
    app.setApplicationName("Ego Limiter"); //set the window name
    MainWindow window; //creating a new "window" object
    window.show(); //show the window
    return app.exec(); //execute the application
}
