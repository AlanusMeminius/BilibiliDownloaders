#pragma once

#include <QObject>
#include <QNetworkAccessManager>

namespace aria2net
{

class AriaClient : public QObject
{
    Q_OBJECT
private:
    static constexpr std::string JSONRPC = "2.0";
    static constexpr std::string TOKEN = "downkyi";

public:
    AriaClient(QObject* parent = nullptr);
    ~AriaClient();

    void Request(const QString& url, const std::string& parameters, int retry = 3);
    inline void Request(const std::string& url, const std::string& parameters, int retry = 3)
    {
        Request(url, parameters, retry);
    }
    
    void Response(QNetworkReply* reply);
protected:
    QNetworkAccessManager* m_networkAccessManager;
};

} // namespace aria2net
