#include "SimpleNetwork.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <nlohmann/json.hpp>
#include <QSslSocket>

#include <ObjBase.h>

#include <QEventLoop>

SimpleNetwork::SimpleNetwork(QObject* parent)
    : QObject(parent)
    , m_networkAccessManager(new QNetworkAccessManager(this))
{
    std::string bvid = "BV117411r7R1";
    std::string baseUrl = "https://api.bilibili.com/x/web-interface/view";
    // std::string referer = "https://www.bilibili.com";
    std::string url;
    url = baseUrl + "?bvid=" + bvid;
    url = "https://qoogwqhm.v1d.szbdyd.com:2996/upgcxcode/93/45/557884593/557884593_nb2-1-30080.m4s?xyip=112.12.135.213&xyport=2996&xy_usource=upos-sz-mirrorhw.bilivideo.com&xy_mark=rsc-upos-sz-mirrorhw.disp.bilivideo.com&e=https://cn-lnsy-cmcc-v-07.bilivideo.com/upgcxcode/50/93/560199350/560199350-1-30280.m4s?e=ig8euxZM2rNcNbdlhoNvNC8BqJIzNbfqXBvEqxTEto8BTrNvN0GvT90W5JZMkX_YN0MvXg8gNEV4NC8xNEV4N03eN0B5tZlqNxTEto8BTrNvNeZVuJ10Kj_g2UB02J0mN0B5tZlqNCNEto8BTrNvNC7MTX502C8f2jmMQJ6mqF2fka1mqx6gqj0eN0B599M=&uipk=5&nbs=1&deadline=1648984585&gen=playurlv2&os=bcache&oi=0&trid=0000d031eb24d14144c99e42d99a7f998433u&platform=pc&upsig=7c5aa131098da3adb05fa05e2518793a&uparams=e,uipk,nbs,deadline,gen,os,oi,trid,platform&cdnid=32707&mid=287031869&bvc=vod&nettype=0&orderid=0,3&agrr=1&bw=40460&logo=80000000\%22,\r\n%20%20%20%20%20%20\%22https://upos-sz-mirrorhw.bilivideo.com/upgcxcode/50/93/560199350/560199350-1-30280.m4s?e=ig8euxZM2rNcNbdlhoNvNC8BqJIzNbfqXBvEqxTEto8BTrNvN0GvT90W5JZMkX_YN0MvXg8gNEV4NC8xNEV4N03eN0B5tZlqNxTEto8BTrNvNeZVuJ10Kj_g2UB02J0mN0B5tZlqNCNEto8BTrNvNC7MTX502C8f2jmMQJ6mqF2fka1mqx6gqj0eN0B599M=&uipk=5&nbs=1&deadline=1648984585&gen=playurlv2&os=hwbv&oi=0&trid=d031eb24d14144c99e42d99a7f998433u&platform=pc&upsig=c6e639a9fdaed0077fcabb1b156ba621&uparams=e,uipk,nbs,deadline,gen,os,oi,trid,platform&mid=287031869&bvc=vod&nettype=0&orderid=1,3&agrr=1&bw=40460&logo=40000000\=&uipk=5&nbs=1&deadline=1648975247&gen=playurlv2&os=rsc&oi=666253600&trid=f9c608da97e442f5870b83b92381a31cu&platform=pc&upsig=ebda3a8537d2e46e5f3db73dbf5ec938&uparams=e,uipk,nbs,deadline,gen,os,oi,trid,platform&pcdnid=1000007207&mid=0&bvc=vod&nettype=0&orderid=0,3&agrr=1&bw=132895&logo=80000000";
    // url = "https://qoogwqjt.v1d.szbdyd.com:13260/upgcxcode/93/45/557884593/557884593_nb2-1-30280.m4s?xyip=112.12.135.195&xyport=13260&xy_usource=upos-sz-mirrorcos.bilivideo.com&xy_mark=rsc-upos-sz-mirrorcos.disp.bilivideo.com&e=ig8euxZM2rNcNbdlhoNvNC8BqJIzNbfqXBvEqxTEto8BTrNvN0GvT90W5JZMkX_YN0MvXg8gNEV4NC8xNEV4N03eN0B5tZlqNxTEto8BTrNvNeZVuJ10Kj_g2UB02J0mN0B5tZlqNCNEto8BTrNvNC7MTX502C8f2jmMQJ6mqF2fka1mqx6gqj0eN0B599M=&uipk=5&nbs=1&deadline=1648975247&gen=playurlv2&os=rsc&oi=666253600&trid=f9c608da97e442f5870b83b92381a31cu&platform=pc&upsig=6a0bac480c1257e817212832ba5b54cd&uparams=e,uipk,nbs,deadline,gen,os,oi,trid,platform&pcdnid=1000007207&mid=0&bvc=vod&nettype=0&orderid=0,3&agrr=1&bw=39953&logo=80000000";
    QNetworkRequest request;
    request.setUrl(QUrl(QString::fromLocal8Bit(url.data())));
    const QByteArray referer("https://www.bilibili.com");
    const QByteArray userAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/90.0.4430.85 Safari/537.36 Edg/90.0.818.49");
    request.setMaximumRedirectsAllowed(0);
    request.setRawHeader("Referer", referer);
    request.setRawHeader("User-Agent", userAgent);
    request.setRawHeader("origin", referer);

    QNetworkReply* reply = m_networkAccessManager->get(request);
    if (reply)
    {
        m_audeoFile.setFileName("test.mp3");
        if (!m_audeoFile.open(QIODevice::WriteOnly))
        {
            return;
        }

        connect(reply, &QNetworkReply::readyRead, [&]() {
            m_audeoFile.write(reply->readAll());
        });

        QEventLoop eventLoop;
        connect(reply, &QNetworkReply::finished, [&]() {
            eventLoop.quit();
        });
        eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
    }

    url = "https://qoogwqjt.v1d.szbdyd.com:13260/upgcxcode/93/45/557884593/557884593_nb2-1-30280.m4s?xyip=112.12.135.195&xyport=13260&xy_usource=upos-sz-mirrorcos.bilivideo.com&xy_mark=rsc-upos-sz-mirrorcos.disp.bilivideo.com&e=ig8euxZM2rNcNbdlhoNvNC8BqJIzNbfqXBvEqxTEto8BTrNvN0GvT90W5JZMkX_YN0MvXg8gNEV4NC8xNEV4N03eN0B5tZlqNxTEto8BTrNvNeZVuJ10Kj_g2UB02J0mN0B5tZlqNCNEto8BTrNvNC7MTX502C8f2jmMQJ6mqF2fka1mqx6gqj0eN0B599M=&uipk=5&nbs=1&deadline=1648975247&gen=playurlv2&os=rsc&oi=666253600&trid=f9c608da97e442f5870b83b92381a31cu&platform=pc&upsig=6a0bac480c1257e817212832ba5b54cd&uparams=e,uipk,nbs,deadline,gen,os,oi,trid,platform&pcdnid=1000007207&mid=0&bvc=vod&nettype=0&orderid=0,3&agrr=1&bw=39953&logo=80000000";

    request.setUrl(QUrl(QString::fromLocal8Bit(url.data())));
    reply = m_networkAccessManager->get(request);
    if (reply)
    {
        m_videoFile.setFileName("test.mp4");
        if (!m_videoFile.open(QIODevice::WriteOnly))
        {
            return;
        }

        connect(reply, &QNetworkReply::readyRead, [&]() {
            m_videoFile.write(reply->readAll());
        });

        QEventLoop eventLoop;
        connect(reply, &QNetworkReply::finished, [&]() {
            eventLoop.quit();
        });
        eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
    }
}

SimpleNetwork::~SimpleNetwork()
{
}
