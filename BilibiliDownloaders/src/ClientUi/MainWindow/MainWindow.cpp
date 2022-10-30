#include <QDir>

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Util/UrlProcess.h"
#include "BiliApi/BilibiliClient.h"
#include "Aria2Net/AriaClient/AriaClient.h"
#include "FFmpeg/FFmpegHelper.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    SetUi();

    // нч╠ъ©Р
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
    BiliApi::PlayUrl playUrl = biliClient.GetPlayUrl(videoView.GetCid(), 125, videoView.GetBvid());

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


    std::this_thread::sleep_for(std::chrono::milliseconds(10000));

    FFmpegHelper::MegerVideo(dirPath.toStdString() + "/" + bvid + ".mp3", 
                             dirPath.toStdString() + "/" + bvid + ".mp4", 
                             dirPath.toStdString() + "/" + bvid + "all.mp4");

}

void MainWindow::SetUi()
{
}

void MainWindow::SignalsAndSlots()
{
    connect(ui->lineEditSearch, &SearchLineEdit::searchCliecked, this, &MainWindow::SearchUrl);
}
