#include "AriaClient.h"

#include <QNetWorkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QFile>

#include <chrono>
#include <thread>
#include <future>

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
    : QThread(parent)
    , m_networkAccessManager(nullptr)
{
    m_networkAccessManager = new QNetworkAccessManager(this);
    connect(m_networkAccessManager, &QNetworkAccessManager::finished, this, &AriaClient::Response);
    
}

aria2net::AriaClient::~AriaClient()
{

}

void aria2net::AriaClient::Request(const QString& url, const QString& parameters, int retry)
{
    QByteArray sendData = parameters.toLocal8Bit();
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, sendData.size());

    QNetworkReply* reply = m_networkAccessManager->post(request, sendData);
}

void aria2net::AriaClient::Request(const std::string& parameters, int retry)
{
    Request(GetRpcUri(), QString("{{\"id\":\"3E7E7D183C08426FBBBD50167B79F94F\",\"jsonrpc\":\"2.0\", \"method\" : \"aria2.addUri\",\"params\" : [\"token:downkyi\", [\"http://dl_dir.qq.com/qqfile/qq/QQ2011/QQ2011.exe\"],{\"dir\":\"D:/workfile/BilibiliDownloaders/x64/Debug/BilibiliDownloaders.exe\",\"out\" : \"qq.exe\" }]}}"), retry);
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

    QString string(rep);

    reply->deleteLater();
    reply = nullptr;

}

void aria2net::AriaClient::run()
{
    exec();
}
