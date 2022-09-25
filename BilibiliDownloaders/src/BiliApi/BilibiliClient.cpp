
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
{
    std::string origin = "origin: ";
    AppendHeaders(origin + mainUrl);
    AppendHeaders("Content-Type: application/json;");
}

} // BiliApi
