#pragma once
#pragma execution_character_set("utf-8")

#include <QObject>

#include <curl/curl.h>

namespace BiliApi
{

class WebClient : public QObject
{
    Q_OBJECT

public:
    enum ResponseType
    {
        Async,
        Sync
    };

    enum ResponseModth
    {
        GET,
        POST
    };

    WebClient(QObject* parent = nullptr);
    ~WebClient();
    
    std::string Request(const std::string& url, ResponseModth modth = GET, const std::string& postData = "", ResponseType type = Sync);
    std::string AsyncRequest(const std::string& url, ResponseModth modth = GET, const std::string& postData = "");

protected:
    CURL* m_curl;          // 请求体
    CURLcode m_curlCode;   // 错误码
    curl_slist* m_headers; // 请求头
    
private:
    void InitHeaders();
};

} // BiliApi

