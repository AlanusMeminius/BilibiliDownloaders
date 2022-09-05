#pragma once
#pragma execution_character_set("utf-8")

#include <QObject>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QThread>

class QDataStream;

namespace aria2net
{

class AriaClient : public QThread
{
    Q_OBJECT
private:
    static constexpr char JSONRPC[] = "2.0";
    static constexpr char TOKEN[] = "downkyi";

public:
    enum ResponseType
    {
        Async,
        Sync
    };

    AriaClient(QObject* parent = nullptr);
    ~AriaClient();

    QByteArray Request(const QString& url, const QString& parameters, int retry = 3, ResponseType responseType = Sync);
    inline QByteArray Request(const std::string& url, const std::string& parameters, int retry = 3, ResponseType responseType = Sync)
    {
        return Request(QString::fromLocal8Bit(url.data()), QString::fromLocal8Bit(parameters.data()), retry, responseType);
    }

    QByteArray Request(const std::string& parameters, int retry = 3);

    QByteArray RequestAsync(const QString& url, const QString& parameters, int retry = 3);
    inline QByteArray RequestAsync(const std::string& url, const std::string& parameters, int retry = 3)
    {
        return RequestAsync(QString::fromLocal8Bit(url.data()), QString::fromLocal8Bit(parameters.data()), retry);
    }

    QByteArray RequestSync(const QString& url, const QString& parameters, int retry = 3);
    inline QByteArray RequestSync(const std::string& url, const std::string& parameters, int retry = 3)
    {
        return RequestSync(QString::fromLocal8Bit(url.data()), QString::fromLocal8Bit(parameters.data()), retry);
    }

    
    static QByteArray Response(QNetworkReply* reply);

    void SetDataStream(QDataStream& dataStream);

protected:
    void run() override;
    QNetworkAccessManager* m_syncNetworkAccessManager;
    QNetworkAccessManager* m_asyncNetworkAccessManager;
};

} // namespace aria2net
