#include <QApplication>
#include <QFile>
#include <QDir>
#include <QDebug>

#include <chrono>
#include <thread>
#include <future>

#include <windows.h>

#include "AriaServer.h"

AriaServer::AriaServer(QObject *parent)
    : QObject(parent)
{
    StartServerAsync();
}

AriaServer::~AriaServer()
{
}

void AriaServer::StartServerAsync()
{
    QString ariaPath = QApplication::applicationDirPath() + "/aria/";
    QString sessionFile = ariaPath + "aira.session";
    QString logFile = ariaPath + "log.txt";

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
        QString startAgr = "--enable-rpc --rpc-listen-all=true --rpc-allow-origin-all=true --rpc-listen-port=6800 --rpc-secret=downkyi "
                           " --input-file=\"D:\\workfile\\downkyi-1.5.0-alpha1\\src\\DownKyi\\bin\\Debug\\aria\\aira.session\" "
                           "--save-session=\"D:\\workfile\\downkyi-1.5.0-alpha1\\src\\DownKyi\\bin\\Debug\\aria\\aira.session\" "
                           "--save-session-interval=30 --log=\"D:\\workfile\\downkyi-1.5.0-alpha1\\src\\DownKyi\\bin\\Debug\\aria\\aira.log\" "
                           "--log-level=info "
                           "--max-concurrent-downloads=3 --max-connection-per-server=16 "
                           "--split=5 --min-split-size=10M --max-overall-download-limit=0 "
                           "--max-download-limit=0 --max-overall-upload-limit=0 --max-upload-limit=0 "
                           "--continue=true --allow-overwrite=true --auto-file-renaming=false --file-allocation=prealloc "
                           "--header=\"Cookie: \" --header=\"Origin : https ://www.bilibili.com\" "
                           "--header=\"Referer: https://www.bilibili.com\" "
                           "--header=\"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/97.0.4692.99 Safari/537.36";
        std::wstring ariaExeWFilename = ariaExefilename.toStdWString();
        std::wstring startWAgr = startAgr.toStdWString();

            //"--rpc-listen-port=6800"
            //"--rpc-secret={config.Token} "
            //"--input-file=\"{sessionFile}\" --save-session=\"{sessionFile}\" " 
            //"--save-session-interval={saveSessionInterval} " 
            //"--log=\"{logFile}\" --log-level={config.LogLevel.ToString("G").ToLower()} "  // log-level: 'debug' 'info' 'notice' 'warn' 'error'
            //"--max-concurrent-downloads={config.MaxConcurrentDownloads} " // 最大同时下载数(任务数)
            //"--max-connection-per-server={config.MaxConnectionPerServer} " // 同服务器连接数
            //"--split={config.Split} "  // 单文件最大线程数
            //                            //$"--max-tries={config.MaxTries} retry-wait=3 " + // 尝试重连次数
            //"--min-split-size={config.MinSplitSize}M "  // 最小文件分片大小, 下载线程数上限取决于能分出多少片, 对于小文件重要
            //"--max-overall-download-limit={config.MaxOverallDownloadLimit} "  // 下载速度限制
            //"--max-download-limit={config.MaxDownloadLimit} "  // 下载单文件速度限制
            //"--max-overall-upload-limit={config.MaxOverallUploadLimit} "  // 上传速度限制
            //"--max-upload-limit={config.MaxUploadLimit} "  // 上传单文件速度限制
            //"--continue={config.ContinueDownload.ToString().ToLower()} "  // 断点续传
            //"--allow-overwrite=true " // 允许复写文件
            //"--auto-file-renaming=false "
            //"--file-allocation={config.FileAllocation.ToString("G").ToLower()} " // 文件预分配, none prealloc
            //"{headers}"  // header
            //"";          // 命令行

        STARTUPINFO si;                // 启动信息
        PROCESS_INFORMATION pi;        // 进程头
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        // 创建子进程，并判断是否成功
        if (!::CreateProcess((LPWSTR)ariaExeWFilename.c_str(),              // 启动程序名
                             (LPWSTR)startWAgr.c_str(),                     // 启参数
                             NULL,                                          // 不继承进程句柄
                             NULL,                                          // 不继承线程句柄
                             FALSE,                                         // 不继承句柄
                             0,                                             // 没有创建标志
                             NULL,                                          // 使用父进程环境变量
                             NULL,                                          // 使用父进程目录作为当前目录
                             &si,                                           // STARTUPINFO 结构
                             &pi))                                          // PROCESS_INFORMATION 保存相关信息
        {
            // 创建失败
            qDebug() << QString("create aria2c.exe failed, error code: %1").arg(::GetLastError());
            return false;
        }

        return true;
    });
}
