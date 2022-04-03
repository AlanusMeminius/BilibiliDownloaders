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
    , m_syncNetworkAccessManager(new QNetworkAccessManager(this))
    , m_asyncNetworkAccessManager(nullptr)
{
    start();
}

aria2net::AriaClient::~AriaClient()
{
    quit();
    wait();
}

QByteArray aria2net::AriaClient::Request(const QString& url, const QString& parameters, int retry, ResponseType responseType)
{
    QByteArray result;

    switch (responseType)
    {
    case aria2net::AriaClient::Async:
        RequestAsync(url, parameters, retry);
        break;
    case aria2net::AriaClient::Sync:
        result = RequestSync(url, parameters, retry);
        break;
    default:
        break;
    }

    return result;
}

QByteArray aria2net::AriaClient::RequestAsync(const QString& url, const QString& parameters, int retry)
{
    QByteArray sendData = parameters.toLocal8Bit();
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, sendData.size());

    QNetworkReply* reply = m_syncNetworkAccessManager->post(request, sendData);

    return QByteArray();
}

QByteArray aria2net::AriaClient::RequestSync(const QString& url, const QString& parameters, int retry)
{
    QByteArray sendData = parameters.toLocal8Bit();
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, sendData.size());


    QNetworkReply* reply = m_syncNetworkAccessManager->post(request, sendData);

    QEventLoop eventLoop;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);      // ×èÈûÇëÇó

    return Response(reply);
}


QByteArray aria2net::AriaClient::Request(const std::string& parameters, int retry)
{
    //QString str = "{\
    //    \"id\": \"A3F386833D5B45D1B2CD706D3123C5C1\",\
    //    \"jsonrpc\" : \"2.0\",\
    //    \"method\" : \"aria2.addUri\",\
    //    \"params\" : [\
    //        \"token:downkyi\",\
    //            [\
    //                \"https://upos-sz-mirrorhw.bilivideo.com/upgcxcode/64/57/549595764/549595764_nb2-1-30280.m4s?e=ig8euxZM2rNcNbdlhoNvNC8BqJIzNbfqXBvEqxTEto8BTrNvN0GvT90W5JZMkX_YN0MvXg8gNEV4NC8xNEV4N03eN0B5tZlqNxTEto8BTrNvNeZVuJ10Kj_g2UB02J0mN0B5tZlqNCNEto8BTrNvNC7MTX502C8f2jmMQJ6mqF2fka1mqx6gqj0eN0B599M=&uipk=5&nbs=1&deadline=1649000446&gen=playurlv2&os=hwbv&oi=0&trid=3b0d12bdd7fa49dcbac36ad281d165e0u&platform=pc&upsig=5bdd320c18cab730053c38b7d977458f&uparams=e,uipk,nbs,deadline,gen,os,oi,trid,platform&mid=287031869&bvc=vod&nettype=0&orderid=0,2&agrr=0&bw=39911&logo=80000000\",\
    //                \"https://upos-sz-mirrorhwb.bilivideo.com/upgcxcode/64/57/549595764/549595764_nb2-1-30280.m4s?e=ig8euxZM2rNcNbdlhoNvNC8BqJIzNbfqXBvEqxTEto8BTrNvN0GvT90W5JZMkX_YN0MvXg8gNEV4NC8xNEV4N03eN0B5tZlqNxTEto8BTrNvNeZVuJ10Kj_g2UB02J0mN0B5tZlqNCNEto8BTrNvNC7MTX502C8f2jmMQJ6mqF2fka1mqx6gqj0eN0B599M=&uipk=5&nbs=1&deadline=1649000446&gen=playurlv2&os=hwbbv&oi=0&trid=3b0d12bdd7fa49dcbac36ad281d165e0u&platform=pc&upsig=f821180ee16ae4a407ec278fadec361b&uparams=e,uipk,nbs,deadline,gen,os,oi,trid,platform&mid=287031869&bvc=vod&nettype=0&orderid=1,2&agrr=0&bw=39911&logo=40000000\"\
    //            ],\
    //        {\
    //          \"out\": \"qq.exe\",\
    //          \"dir\" : \"D:/workfile/BilibiliDownloaders/x64/Debug\"\
    //        }\
    //    ]\
    //}";
    return Request(GetRpcUri(), QString::fromLocal8Bit(parameters.data()), retry);
}

QByteArray aria2net::AriaClient::Response(QNetworkReply* reply)
{
    
    if (reply == nullptr)
    {
        LogInfo("AriaClient Response is null");
        return QByteArray();
    }
    if (reply->error() == QNetworkReply::OperationCanceledError) 
    {
        LogError("AriaClient Response is error, errorType: OperationCanceledError");
        reply->abort();
        return QByteArray();
    }
    if (reply->error() != QNetworkReply::NoError) 
    {
        LogError("AriaClient Response is error, errorMsg: {}  url={} ", reply->errorString().toStdString(), reply->url().toString().toStdString());
        return QByteArray();
    }


    LogInfo("reply from {}",  reply->url().toString().toStdString());

    QByteArray rep = reply->readAll();

    reply->deleteLater();
    reply = nullptr;

    return rep;
}

void aria2net::AriaClient::SetDataStream(QDataStream& dataStream)
{
    //m_asyncDataStream = dataStream;
}

void aria2net::AriaClient::run()
{
    m_asyncNetworkAccessManager = new QNetworkAccessManager(this);
    connect(m_asyncNetworkAccessManager, &QNetworkAccessManager::finished, this, &AriaClient::Response);
    exec();
}
