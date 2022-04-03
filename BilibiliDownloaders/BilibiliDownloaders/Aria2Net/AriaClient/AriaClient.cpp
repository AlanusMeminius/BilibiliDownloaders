#include "AriaClient.h"

#include <QNetWorkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QFile>

#include <nlohmann/json.hpp>

#include "loger/Loger.h"

namespace aria2net
{

const QString GetRpcUri(int listenPort = 6800)
{
    return QString("http://localhost:%1/jsonrpc").arg(listenPort);
}

} // namespace aria2net



aria2net::AriaClient::AriaClient(QObject *parent)
    : QObject(parent)
    , m_networkAccessManager(new QNetworkAccessManager(this))
{
}

aria2net::AriaClient::~AriaClient()
{
}

void aria2net::AriaClient::Request(const QString& url, const std::string& parameters, int retry)
{
    QByteArray sendData(parameters.data());
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, sendData.size());

    QNetworkReply* reply = m_networkAccessManager->post(request, sendData);


}

void aria2net::AriaClient::Response(QNetworkReply* reply)
{
    
    if (reply == nullptr)
    {
        LogInfo("AriaClient Response is null");
        return;
    }
    if (reply->error() == QNetworkReply::OperationCanceledError) 
    {
        LogError("AriaClient Response is error, errorType: OperationCanceledError");
        reply->abort();
        return;
    }
    if (reply->error() != QNetworkReply::NoError) 
    {
        LogError("AriaClient Response is error, errorMsg: {}  url={} ", reply->errorString().toStdString(), reply->url().toString().toStdString());
        return;
    }


    LogInfo("reply from {}",  reply->url().toString().toStdString());

    QByteArray rep = reply->readAll();

    QFile file("text.mp4");
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(rep);
    }
    file.close();


    reply->deleteLater();
    reply = nullptr;
}
