#pragma once

#include <QObject>

#include <curl/curl.h>

#include <nlohmann/json.hpp>

#include <string>

#include "NetWork/CNetWork.h"
#include "BiliApi.h"

namespace BiliApi
{

constexpr char const mainUrl[] = "https://www.bilibili.com/";
constexpr char const videoViewUrl[] = "http://api.bilibili.com/x/web-interface/view";
constexpr char const videoPlayUrl[] = "https://api.bilibili.com/x/player/playurl";

class BilibiliClient : public CNetWork
{
public:
    static BilibiliClient& GetInstance();

    VideoView GetVideoView(const std::string& bvid);
    PlayUrl GetPlayUrl(long long cid, long long qn, const std::string& bvid);

    
    static nlohmann::json GetDataFromRespones(const std::string& respones);
private:
    BilibiliClient();
    ~BilibiliClient() = default;

    // µ¥Àý½ûÖ¹
    BilibiliClient(const BilibiliClient& other) = delete;
    BilibiliClient& operator=(const BilibiliClient& other) = delete;
    BilibiliClient(BilibiliClient&& other) = delete;
    BilibiliClient& operator=(BilibiliClient&& other) = delete;

     
};

} // BiliApi

