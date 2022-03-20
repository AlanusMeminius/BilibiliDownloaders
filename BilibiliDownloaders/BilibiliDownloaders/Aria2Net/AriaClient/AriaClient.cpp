#include "AriaClient.h"

#include <QNetWorkAccessManager>
#include <QNetworkReply>
#include <QUrl>

const QString GetRpcUri(int listenPort = 6800)
{
    return QString("http://localhost:%1/jsonrpc").arg(listenPort);
}

AriaClient::AriaClient(QObject *parent)
    : QObject(parent)
{
}

AriaClient::~AriaClient()
{
}
