#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QThread>

namespace aria2net
{

class AriaClient : public QThread
{
    Q_OBJECT
private:
    static constexpr std::string JSONRPC = "2.0";
    static constexpr std::string TOKEN = "downkyi";

public:
    AriaClient(QObject* parent = nullptr);
    ~AriaClient();

    void Request(const QString& url, const QString& parameters, int retry = 3);
    void Request(const std::string& parameters, int retry = 3);

    
    void Response(QNetworkReply* reply);

protected:
    void run() override;
    QNetworkAccessManager* m_networkAccessManager;
};

} // namespace aria2net
