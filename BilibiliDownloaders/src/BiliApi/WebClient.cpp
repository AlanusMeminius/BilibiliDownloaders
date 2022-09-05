#include <QNetWorkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QFile>
#include <QByteArray>
#include <QEventLoop>

#include "loger\Loger.h"
#include "WebClient.h"

namespace BiliApi
{

namespace
{
    constexpr char chrome[] = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/97.0.4692.99 Safari/537.36";
    constexpr char firefox[] = "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:68.0) Gecko/20100101 Firefox/68.0";
    constexpr char edge[] = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36 Edge/16.16299";
    constexpr char opera[] = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36 OPR/45.0.2552.8 (EdgE)";
    constexpr char ie[] = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36 Edge/16.16299";
    constexpr char safari[] = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36 OPR/45.0.2552.8 (EdgE)";
    
    constexpr char const accept[] = "Accept: application/json";
    constexpr char const accept_encoding[] = "Accept-Encoding: gzip, deflate, br";
    constexpr char const accept_language[] = "Accept-Language: zh-CN,zh;q=0.9,en-US;q=0.8,en;q=0.7";
    constexpr char const connect_type[] = "Connection: keep-alive";

    constexpr char const mainUrl[] = "https://bilibili.com/";
}
   
WebClient::WebClient(QObject* parent)
    : QObject(parent)
    , m_curl(nullptr)
    , m_headers(nullptr)
{
    curl_global_init(CURL_GLOBAL_ALL);
    m_curl = curl_easy_init();
    InitHeaders();
}

WebClient::~WebClient()
{
    curl_easy_cleanup(m_curl);
    curl_global_cleanup();
}

std::string WebClient::Request(const std::string& url, ResponseModth modth, const std::string& postData, ResponseType type)
{
    std::string response;
    curl_easy_setopt(m_curl, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(m_curl, CURLOPT_HEADER, 0);
    curl_easy_setopt(m_curl, CURLOPT_POST, 1);
    curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 0);
    curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(m_curl, CURLOPT_REFERER, mainUrl);
    curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, 5);
    //…Ë÷√ssl—È÷§
    curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, false);
    curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, false);
    m_curlCode = curl_easy_perform(m_curl);
    return response;
}

std::string WebClient::AsyncRequest(const std::string& url, ResponseModth modth, const std::string& postData)
{
    return Request(url, modth, postData, Async);
}

void WebClient::InitHeaders()
{
    std::string userAgent = std::string("user-agent: ") + chrome;
    // m_headers = curl_slist_append(m_headers, accept);
    m_headers = curl_slist_append(m_headers, accept_encoding);
    m_headers = curl_slist_append(m_headers, accept_language);
    m_headers = curl_slist_append(m_headers, userAgent.c_str());
}



} // BiliApi
