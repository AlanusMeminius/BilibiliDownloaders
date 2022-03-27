#include "SimpleNetwork.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <nlohmann/json.hpp>
#include <QSslSocket>

SimpleNetwork::SimpleNetwork(QObject *parent)
    : QObject(parent)
    , m_networkAccessManager(new QNetworkAccessManager(this))
{
    std::string bvid = "BV117411r7R1";
    std::string baseUrl = "https://api.bilibili.com/x/web-interface/view";
    // std::string referer = "https://www.bilibili.com";
    std::string url;
    url = baseUrl + "?bvid=" + bvid;
    // qDebug() << url;

    QNetworkRequest request;
    request.setUrl(QUrl(QString::fromLocal8Bit(url.data())));
    // request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QString strVersion = QSslSocket::sslLibraryBuildVersionString();

    const QByteArray referer("https://www.bilibili.com");
    const QByteArray userAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/90.0.4430.85 Safari/537.36 Edg/90.0.818.49");
    request.setMaximumRedirectsAllowed(0);
    request.setRawHeader("Referer", referer);
    request.setRawHeader("User-Agent", userAgent);
    request.setRawHeader("origin", referer);

    QNetworkReply* reply = m_networkAccessManager->get(request);
    if (reply)
    {
        connect(m_networkAccessManager, &QNetworkAccessManager::finished, [&](QNetworkReply* reply) {

                // abort() is called.
                if (reply->error() == QNetworkReply::OperationCanceledError) {
                    reply->abort();
                    return;
                }
                if (reply->error() != QNetworkReply::NoError) {
                    qDebug() << "network error:" << reply->errorString() << ", url=" << reply->url().toString();
                    return;
                }
                //if (!reply->header(QNetworkRequest::ContentTypeHeader).toString().contains("json")) {
                //    return;
                //}

                qDebug() << "reply from" << reply->url();
                QByteArray rep = reply->readAll();

                std::string strRep = QString::fromUtf8(rep).toStdString();
                qDebug() << QString::fromUtf8(rep);
                if (QString::fromUtf8(rep).contains("message"))
                {
                    nlohmann::json json = nlohmann::json::parse(strRep);
                    std::string bvide = json["data"]["bvid"];
                    qDebug() << "ok";
                }


                reply->deleteLater();
                reply = nullptr;
        });
    }
    
    
}

SimpleNetwork::~SimpleNetwork()
{
}
