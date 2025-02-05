#include <QFile>
#include <QApplication>

#include "loger\Loger.h"
#include "BilibiliClient.h"



namespace BiliApi
{
    

BilibiliClient& BilibiliClient::GetInstance()
{
    static BilibiliClient bilibilClient;
    return bilibilClient;
}

VideoView BilibiliClient::GetVideoView(const std::string& bvid)
{
    std::string url = videoViewUrl + std::string("?bvid=") + bvid;
    std::string response;
    HttpGet(url, response);
    return VideoView(GetDataFromRespones(response));
}

PlayUrl BilibiliClient::GetPlayUrl(long long cid, long long qn, const std::string& bvid)
{
    // "https://api.bilibili.com/x/player/playurl?cid=%s&qn=125&fourk=1&fnver=0&fnval=4048&bvid=%s";
    std::string url = videoPlayUrl + std::string("?cid=") + std::to_string(cid) +
        "&qn=" + std::to_string(qn) + "&fourk=1&fnver=0&fnval=4048&bvid=" + bvid;
    std::string response;
    HttpGet(url, response);
    return PlayUrl(GetDataFromRespones(response));
}

LoginUrlOrigin BilibiliClient::GetLoginUrl()
{
    std::string url = "https://passport.bilibili.com/qrcode/getLoginUrl";
    std::string response;

    curl_slist_free_all(m_headers);
    m_headers = nullptr;
    InitDefaultHeadersLogin();
    HttpGet(url, response);
    curl_slist_free_all(m_headers);
    m_headers = nullptr;
    InitDefaultHeaders();

    nlohmann::json json;
    try 
    {
        json = nlohmann::json::parse(response);
    }
    catch (std::exception& e) 
    {
    }
    return LoginUrlOrigin(json);
}

LoginStatusScanning BilibiliClient::GetLoginStatus(const std::string& oauthKey)
{
    std::string url = "https://passport.bilibili.com/qrcode/getLoginInfo";
    std::string response;

    curl_slist_free_all(m_headers);
    m_headers = nullptr;
    InitDefaultHeadersLogin();
    ParamType param{
        {"oauthKey", oauthKey}, 
        {"gourl", "https://www.bilibili.com"},
    };
    url = url + "?oauthKey=" + oauthKey + "&gourl=https://www.bilibili.com";
    HttpPost(url, param, response);
    curl_slist_free_all(m_headers);
    m_headers = nullptr;
    InitDefaultHeaders();

    nlohmann::json json;
    try 
    {
        json = nlohmann::json::parse(response);
    }
    catch (std::exception& e) 
    {
        return {};
    }
    return LoginStatusScanning(json);
}

void BilibiliClient::SetLogined(bool logined) 
{
    m_logined = logined;
}

bool BilibiliClient::GetLogined() const
{
    return m_logined;
}

nlohmann::json BilibiliClient::GetDataFromRespones(const std::string& respones)
{
    nlohmann::json json;
    try
    {
        json = nlohmann::json::parse(respones);
    }
    catch(std::exception& e)
    {

    }
    return json["data"];
}

BilibiliClient::BilibiliClient()
    : CNetWork()
    , m_logined(false)
{
    std::string origin = "origin: ";
    AppendHeaders(origin + mainUrl);
    AppendHeaders("Content-Type: application/json;");
}

} // BiliApi
