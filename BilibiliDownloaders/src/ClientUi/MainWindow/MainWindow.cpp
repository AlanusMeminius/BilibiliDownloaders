#include <QDir>

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Util/UrlProcess.h"
#include "BiliApi/BilibiliClient.h"
#include "Aria2Net/AriaClient/AriaClient.h"
#include "FFmpeg/FFmpegHelper.h"
#include "Config/SingleConfigRander.h"
#include "Sqlite/SQLiteManager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SetUi();

    SingleConfigRander::getInstance();
    SQLite::SQLiteManager::getInstance();
    // �ޱ߿�
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    setWindowTitle(tr("Bili Downloader"));
    SignalsAndSlots();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SearchUrl()
{
    static QString strLast;
    QString input = ui->lineEditSearch->text();

    if (input == strLast)
    {
        return;
    }
    strLast = input;

    bool bBvUrl = util::UrlProcess::IsBvUrl(input);
    if (!bBvUrl)
    {
        return;
    }

    std::string bvid = util::UrlProcess::GetVideoId(input).toLocal8Bit().toStdString();

    BiliApi::BilibiliClient& biliClient = BiliApi::BilibiliClient::GetInstance();
    BiliApi::VideoView videoView = biliClient.GetVideoView(bvid);
    BiliApi::PlayUrl playUrl = biliClient.GetPlayUrl(videoView.cid, 125, videoView.bvid);

    std::list<std::string> video_urls;
    std::list<std::string> audio_urls;
    auto videos = playUrl.dash.video;
    for (const auto& video : videos)
    {
        video_urls.push_back(video.base_url);
    }
    auto audioes = playUrl.dash.audio;
    for (const auto& audio : audioes)
    {
        audio_urls.push_back(audio.base_url);
    }

    QDir dir("Download");
    QString dirPath = dir.absolutePath();
    if (!dir.exists())
    {
        dir.mkpath(dirPath);
    }


    aria2net::AriaSendOption ariaSendOption;
    ariaSendOption.SetDir(dirPath.toStdString());
    ariaSendOption.SetOut(bvid + ".mp4");


    aria2net::AriaClient& ariaClient = aria2net::AriaClient::GetInstance();
    auto ariaAddUriVideo =  ariaClient.AddUriAsync(video_urls, ariaSendOption);
    ariaSendOption.SetOut(bvid + ".mp3");
    auto ariaAddUriAudio = ariaClient.AddUriAsync(audio_urls, ariaSendOption);

    auto json = ariaAddUriVideo.GetJson();
    // qDebug() << json["result"].c_str();

    auto future =  std::async(std::launch::async, [&, ariaAddUriVideo, ariaAddUriAudio]() {
        bool videoOk = false, audioOk = false;
        while (true)
        {
            auto json = ariaAddUriVideo.GetJson();
            std::string str = json["result"];

            auto status = ariaClient.TellStatus(str);
            if (status.GetResult().GetStatus() == "complete")
            {
                videoOk = true;
            }

            json = ariaAddUriAudio.GetJson();
            str = json["result"];
            status = ariaClient.TellStatus(str);
            if (status.GetResult().GetStatus() == "complete")
            {
                audioOk = true;
            }

            if (audioOk && videoOk)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                FFmpegHelper::MegerVideo(dirPath.toStdString() + "/" + bvid + ".mp3",
                    dirPath.toStdString() + "/" + bvid + ".mp4",
                    dirPath.toStdString() + "/" + bvid + "all.mp4");
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    });

    future.get();
}

void MainWindow::SetUi()
{
}

void MainWindow::SignalsAndSlots()
{
    connect(ui->lineEditSearch, &SearchLineEdit::searchCliecked, this, &MainWindow::SearchUrl);
}
