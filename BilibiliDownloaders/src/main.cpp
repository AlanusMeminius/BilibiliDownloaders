#pragma execution_character_set("utf-8")

#include <codecvt>

#include "loger/dump.h"
#include "ClientUi/MainWindow/MainWindow.h"
#include "BiliApi/WebClient.h"
#include "BiliApi/BiliApi.h"
#include "Util/UrlProcess.h"
#include "Aria2Net/AriaServer/AriaServer.h"
#include "nlohmann/json.hpp"
#include "Aria2Net/AriaClient/AriaClient.h"
#include "NetWork/CNetWork.h"
#include "Aria2Net/Protocol/Protocol.h"
#include "NetWork/NetworkLog.h"
#include "FFmpeg/FFmpegHelper.h"

#include <QNetWorkReply>
#include <QNetWorkRequest>
#include <QNetworkAccessManager>
#include <QFile>
#include <QDir>
#include <QUuid>

std::string gb2312_to_utf8(std::string const& strGb2312)
{
    std::vector<wchar_t> buff(strGb2312.size());
#ifdef _MSC_VER
    std::locale loc("zh-CN");
#else
    std::locale loc("zh_CN.GB18030");
#endif
    wchar_t* pwszNext = nullptr;
    const char* pszNext = nullptr;
    mbstate_t state = {};
    int res = std::use_facet<std::codecvt<wchar_t, char, mbstate_t> >
        (loc).in(state,
            strGb2312.data(), strGb2312.data() + strGb2312.size(), pszNext,
            buff.data(), buff.data() + buff.size(), pwszNext);
    if (std::codecvt_base::ok == res)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> cutf8;
        return cutf8.to_bytes(std::wstring(buff.data(), pwszNext));
    }
    return "";
}

#include<iostream>
#include<string>
#include<curl\curl.h>
#include "loger/loger.h"

int main(int argc, char *argv[])
{
    SetUnhandledExceptionFilter(ExceptionFilter);

    QApplication a(argc, argv);
    
    Logger::GetInstance();

    NETWORK_LOG_TRACE("trace");
    NETWORK_LOG_DEBUG("debug");
    NETWORK_LOG_INFO("info");
    NETWORK_LOG_ERROR("error");
    NETWORK_LOG_WARN("warn");
    NETWORK_LOG_CRITICAL("critical");

    aria2net::AriaServer AriaServer;
    const std::string url = "https://www.bilibili.com/video/BV1CT411V79S?spm_id_from=333.999.0.0";

    QString qstrUrl = QString::fromLocal8Bit(url.data());
    QString bvid = util::UrlProcess::GetId(qstrUrl);

    std::string baseUrl = "https://api.bilibili.com/x/web-interface/view";
    std::string url1 = baseUrl + "?bvid=" + bvid.toStdString();

    BiliApi::WebClient webclient;
    
    //std::string reponse = webclient.Request(url1);
    QNetworkRequest request(QString::fromLocal8Bit(url1.data()));
    std::string reponse1;
    CNetWork::GetInstance().HttpGet(url1, reponse1);
    QFile file("test.txt");
    file.open(QIODevice::WriteOnly);
    file.write(reponse1.data());
    file.close();
    auto test1 = nlohmann::json::parse(reponse1);

    BiliApi::VideoView vidioView(test1["data"]);

    auto aid =  vidioView.GetAid();
    auto de = vidioView.GetDimension();
    auto page = vidioView.GetPages();

    std::string VideoUrl = "https://api.bilibili.com/x/player/playurl?cid=%s&qn=125&fourk=1&fnver=0&fnval=4048&bvid=%s";
    char url2[1000];

    std::sprintf(url2, VideoUrl.c_str(), std::to_string(vidioView.GetCid()).c_str(), vidioView.GetBvid().c_str());
    reponse1.clear();
    CNetWork::GetInstance().HttpGet(url2, reponse1);
    QFile file1("test1.txt");
    file1.open(QIODevice::WriteOnly);
    file1.write(reponse1.data());
    file1.close();
    auto test2 = nlohmann::json::parse(reponse1);
    BiliApi::PlayUrl playUrl(test2["data"]);

    QDir dir("Download");
    QString dirPath = dir.absolutePath();
    if (!dir.exists())
    {
        dir.mkpath(dirPath);
    }

    std::list<std::string> video_urls;
    std::list<std::string> audio_urls;
    auto videos = playUrl.GetDash().GetVideo();
    for (const auto& video : videos)
    {
        video_urls.push_back(video.GetBaseUrl());
    }
    auto audioes = playUrl.GetDash().GetAudio();
    for (const auto& audio : audioes)
    {
        audio_urls.push_back(audio.GetBaseUrl());
    }

    aria2net::AriaSendOption ariaSendOption;
    ariaSendOption.SetDir(dirPath.toStdString());
    ariaSendOption.SetOut("video.mp4");



    aria2net::AriaClient ariaClient;
    ariaClient.AddUriAsync(video_urls, ariaSendOption);
    ariaSendOption.SetOut("audio.mp4");
    ariaClient.AddUriAsync(audio_urls, ariaSendOption);

    std::list<std::string> url21 = {
        "http://i1.hdslb.com/bfs/storyff/n220911a23aymuwfy5s6nituxlijfls4_firsti.jpg",
    };

    aria2net::AriaSendOption ariaSendOption1;
    ariaSendOption1.SetDir(dirPath.toStdString());
    ariaSendOption1.SetOut("cover1.jpg");
    ariaClient.AddUriAsync(url21, ariaSendOption1);

    std::this_thread::sleep_for(std::chrono::milliseconds(10000));

    FFmpegHelper::MegerVideo(dirPath.toStdString() + "/audio.mp4", dirPath.toStdString() + "/video.mp4", dirPath.toStdString() + "/all.mp4");

    MainWindow w;
    w.show();
    return a.exec();
}


