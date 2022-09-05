#include "FFmpegHelper.h"

#include <QApplication>

#include <chrono>
#include <thread>
#include <future>

#include <QString>
#include <QDebug>

namespace {
    constexpr char ffmpegCommand[] = "-i \"%1\" -i \"%2\" -acodec copy -vcodec copy -f mp4 \"%3\"";
}

FFmpegHelper::FFmpegHelper()
    : m_pi{0}
{
}

FFmpegHelper::~FFmpegHelper()
{
    // CloseFFmpeg();
}

bool FFmpegHelper::MegerVideo(const std::string& audio, const std::string& video, const std::string& destionVideo)
{
    FFmpegHelper ffmpegHelper;
    ffmpegHelper.StartFFpmegAsync(audio, video, destionVideo);
    return true;
}

void FFmpegHelper::StartFFpmegAsync(const std::string& audio, const std::string& video, const std::string& destionVideo)
{
    std::future<bool> result = std::async(std::launch::async, [&]()->bool {
        STARTUPINFO si;                // 启动信息
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        QString ffmpegExeFilename;
        ffmpegExeFilename = QApplication::applicationDirPath() + "/ffmpeg.exe";
        QString ffmpegArg(ffmpegCommand);
        ffmpegArg = ffmpegArg.arg(audio.c_str()).arg(video.c_str()).arg(destionVideo.c_str());
        qDebug() << ffmpegArg;

        // 创建子进程，并判断是否成功
        if (!::CreateProcess((LPWSTR)ffmpegExeFilename.toStdWString().c_str(),              // 启动程序名
            (LPWSTR)ffmpegArg.toStdWString().c_str(),                     // 启参数
            NULL,                                          // 不继承进程句柄
            NULL,                                          // 不继承线程句柄
            FALSE,                                         // 不继承句柄
            0,                                             // 没有创建标志
            NULL,                                          // 使用父进程环境变量
            NULL,                                          // 使用父进程目录作为当前目录
            &si,                                           // STARTUPINFO 结构
            &m_pi))                                        // PROCESS_INFORMATION 保存相关信息
        {
            // 创建失败
            qDebug() << QString("create ffmpeg.exe failed, error code: %1").arg(::GetLastError());
            return false;
        }

        return true;
    });
}

void FFmpegHelper::CloseFFmpeg()
{
    if (m_pi.hThread != nullptr && m_pi.hThread != INVALID_HANDLE_VALUE)
    {
        ::CloseHandle(m_pi.hThread);
        m_pi.hThread = nullptr;
    }

    if (m_pi.hProcess != nullptr && m_pi.hProcess != INVALID_HANDLE_VALUE)
    {
        ::CloseHandle(m_pi.hProcess);
        m_pi.hProcess = nullptr;
    }
}
