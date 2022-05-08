#pragma execution_character_set("utf-8")

#include "MainWindow/MainWindow.h"
#include "AriaServer.h"

int main(int argc, char *argv[])
{
    aria2net::AriaServer AriaServer;

    QApplication a(argc, argv);
    
    MainWindow w;
    w.show();
    return a.exec();
}


