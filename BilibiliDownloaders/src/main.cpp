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

#include <QNetWorkReply>
#include <QNetWorkRequest>
#include <QNetworkAccessManager>
#include <QFile>
#include <QDir>

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

namespace {
    std::string GenerateGuid()
    {
        GUID guid;
        CoCreateGuid(&guid);
        char cBuffer[64] = { 0 };
        sprintf_s(cBuffer, sizeof(cBuffer),
            "%08X%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X",
            guid.Data1, guid.Data2,
            guid.Data3, guid.Data4[0],
            guid.Data4[1], guid.Data4[2],
            guid.Data4[3], guid.Data4[4],
            guid.Data4[5], guid.Data4[6],
            guid.Data4[7]);

        return std::string(cBuffer);
    }
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

    NetWork_LOG_TRACE("trace");
    NetWork_LOG_DEBUG("debug");
    NetWork_LOG_INFO("info");
    NetWork_LOG_ERROR("error");
    NetWork_LOG_WARN("warn");
    NetWork_LOG_CRITICAL("critical");

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

    std::list<std::string> urls;
    auto videos = playUrl.GetDash().GetVideo();
    for (const auto& video : videos)
    {
        urls.push_back(video.GetBaseUrl());
    }

    aria2net::AriaSendOption ariaSendOption;
    ariaSendOption.SetDir(dirPath.toStdString());
    ariaSendOption.SetOut("download.mp4");

    nlohmann::json params = urls;
    std::list<std::string> strParams;
    strParams.emplace_back("token:downkyi");
    strParams.emplace_back(params.dump());
    strParams.emplace_back(ariaSendOption.toString());


    aria2net::AriaSendData sendDate;
    std::string id = GenerateGuid();
    sendDate.SetId(GenerateGuid());
    sendDate.SetMethod("aria2.addUri");
    sendDate.SetJsonrpc("2.0");
    sendDate.SetParams(strParams);

    aria2net::AriaClient ariaClient;
    ariaClient.Request(sendDate.toString());


    MainWindow w;
    w.show();
    return a.exec();
}


