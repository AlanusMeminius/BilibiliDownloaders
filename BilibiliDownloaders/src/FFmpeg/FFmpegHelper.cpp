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
        STARTUPINFO si;                // ������Ϣ
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        QString ffmpegExeFilename;
        ffmpegExeFilename = QApplication::applicationDirPath() + "/ffmpeg.exe";
        QString ffmpegArg(ffmpegCommand);
        ffmpegArg = ffmpegArg.arg(audio.c_str()).arg(video.c_str()).arg(destionVideo.c_str());
        qDebug() << ffmpegArg;

        // �����ӽ��̣����ж��Ƿ�ɹ�
        if (!::CreateProcess((LPWSTR)ffmpegExeFilename.toStdWString().c_str(),              // ����������
            (LPWSTR)ffmpegArg.toStdWString().c_str(),                     // ������
            NULL,                                          // ���̳н��̾��
            NULL,                                          // ���̳��߳̾��
            FALSE,                                         // ���̳о��
            0,                                             // û�д�����־
            NULL,                                          // ʹ�ø����̻�������
            NULL,                                          // ʹ�ø�����Ŀ¼��Ϊ��ǰĿ¼
            &si,                                           // STARTUPINFO �ṹ
            &m_pi))                                        // PROCESS_INFORMATION ���������Ϣ
        {
            // ����ʧ��
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
