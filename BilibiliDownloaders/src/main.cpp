#include <QApplication>

#include "Test/SimpleNetwork.h"
#include "loger/dump.h"
#include "ClientUi/MainWindow/MainWindow.h"
#include "Aria2Net/AriaServer/AriaServer.h"

int main(int argc, char *argv[])
{
    SetUnhandledExceptionFilter(ExceptionFilter);

    QApplication a(argc, argv);
    
    aria2net::AriaServer ariaServer;
    ariaServer.StartServerAsync();

    MainWindow w;
    w.show();
    return a.exec();
}


