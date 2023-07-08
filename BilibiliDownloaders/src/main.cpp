#include <QApplication>
#include <QTranslator>

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

    QString strAppPath = QApplication::applicationDirPath();

    auto translator1 = std::make_shared<QTranslator>();
    translator1->load(strAppPath + "/translations/bilibilidownloaders_zh.qm");
    qApp->installTranslator(translator1.get());
    auto translator2 = std::make_shared<QTranslator>();
    translator2->load(strAppPath + "/translations/qt_zh_CN.qm");
    qApp->installTranslator(translator2.get());

    MainWindow w;
    w.show();
    return a.exec();
}


