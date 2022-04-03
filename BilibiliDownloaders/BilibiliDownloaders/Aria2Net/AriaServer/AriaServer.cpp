#include <QApplication>
#include <QFile>
#include <QDir>
#include <QDebug>

#include <chrono>
#include <thread>
#include <future>

#include <windows.h>

#include "AriaServer.h"

namespace aria2net {

AriaServer::AriaServer(QObject *parent)
    : QObject(parent)
    , m_pi{0}
{
    StartServerAsync();
}

AriaServer::~AriaServer()
{
    CloseServer();
}

void AriaServer::StartServerAsync()
{
    QString ariaPath = QApplication::applicationDirPath() + "/aria/";
    QString sessionFile = ariaPath + "aira.session";
    QString logFile = ariaPath + "log.txt";

    // 异步启动多线程下载器
    std::future<bool> result = std::async(std::launch::async, [&]()->bool {
        QDir airDir = QDir(ariaPath);
        bool exist = airDir.exists();
        if (!exist)
        {
            bool ok = airDir.mkpath(ariaPath);
            if (!ok)
            {
                return ok;
            }
        }

        exist = QFile::exists(sessionFile);
        if (!exist)
        {
            QFile file(sessionFile);
            file.open(QIODevice::ReadWrite | QIODevice::Text);
            file.close();
        }

        exist = QFile::exists(logFile);
        if (!exist)
        {
            QFile file(logFile);
            file.open(QIODevice::ReadWrite | QIODevice::Text);
            file.close();
        }

        QString ariaExefilename = ariaPath + "aria2c.exe";
        QString startAgr = "--enable-rpc --rpc-listen-all=true --rpc-allow-origin-all=true --rpc-listen-port=6800 --rpc-secret=downkyi"
                           " --input-file=\"%1\" "
                           "--save-session=\"%1\" "
                           "--save-session-interval=30 --log=\"%2\" "
                           "--log-level=info "
                           "--max-concurrent-downloads=3 --max-connection-per-server=16 "
                           "--split=5 --min-split-size=10M --max-overall-download-limit=0 "
                           "--max-download-limit=0 --max-overall-upload-limit=0 --max-upload-limit=0 "
                           "--continue=true --allow-overwrite=true --auto-file-renaming=false --file-allocation=prealloc "
                           "--header=\"Cookie: \" --header=\"Origin : https ://www.bilibili.com\" "
                           "--header=\"Referer: https://www.bilibili.com\" "
                           "--header=\"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/97.0.4692.99 Safari/537.36";
        startAgr = startAgr.arg(ariaPath).arg(logFile);
        std::wstring ariaExeWFilename = ariaExefilename.toStdWString();
        std::wstring startWAgr = startAgr.toStdWString();

        STARTUPINFO si;                // 启动信息
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);

        // 创建子进程，并判断是否成功
        if (!::CreateProcess((LPWSTR)ariaExeWFilename.c_str(),              // 启动程序名
                             (LPWSTR)startWAgr.c_str(),                     // 启参数
                             NULL,                                          // 不继承进程句柄
                             NULL,                                          // 不继承线程句柄
                             FALSE,                                         // 不继承句柄
                             CREATE_NEW_CONSOLE,                            // 没有创建标志
                             NULL,                                          // 使用父进程环境变量
                             NULL,                                          // 使用父进程目录作为当前目录
                             &si,                                           // STARTUPINFO 结构
                             &m_pi))                                        // PROCESS_INFORMATION 保存相关信息
        {
            // 创建失败
            qDebug() << QString("create aria2c.exe failed, error code: %1").arg(::GetLastError());
            return false;
        }

        return true;
    });
}

// 强制关闭
void AriaServer::CloseServer()
{
    if (m_pi.hThread != nullptr && m_pi.hThread != INVALID_HANDLE_VALUE)
    {
        ::CloseHandle(m_pi.hThread);
    }

    if (m_pi.hProcess != nullptr && m_pi.hProcess != INVALID_HANDLE_VALUE)
    {
        ::CloseHandle(m_pi.hProcess);
    }
}

} // namespace aria2net
