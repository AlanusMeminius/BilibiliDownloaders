#include <QApplication>
#include <QFile>
#include <QDir>

#include <chrono>
#include <thread>
#include <future>

#include <windows.h>

#include "AriaServer.h"

AriaServer::AriaServer(QObject *parent)
    : QObject(parent)
{
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
        QString startAgr = 
            QString("--enable-rpc --rpc-listen-all=true --rpc-allow-origin-all=true ") +
            "--rpc-listen-port=6800"
            "--rpc-secret={config.Token} "
            "--input-file=\"{sessionFile}\" --save-session=\"{sessionFile}\" " 
            "--save-session-interval={saveSessionInterval} " 
            "--log=\"{logFile}\" --log-level={config.LogLevel.ToString("G").ToLower()} "  // log-level: 'debug' 'info' 'notice' 'warn' 'error'
            "--max-concurrent-downloads={config.MaxConcurrentDownloads} " // 最大同时下载数(任务数)
            "--max-connection-per-server={config.MaxConnectionPerServer} " // 同服务器连接数
            "--split={config.Split} "  // 单文件最大线程数
                                        //$"--max-tries={config.MaxTries} retry-wait=3 " + // 尝试重连次数
            "--min-split-size={config.MinSplitSize}M "  // 最小文件分片大小, 下载线程数上限取决于能分出多少片, 对于小文件重要
            "--max-overall-download-limit={config.MaxOverallDownloadLimit} "  // 下载速度限制
            "--max-download-limit={config.MaxDownloadLimit} "  // 下载单文件速度限制
            "--max-overall-upload-limit={config.MaxOverallUploadLimit} "  // 上传速度限制
            "--max-upload-limit={config.MaxUploadLimit} "  // 上传单文件速度限制
            "--continue={config.ContinueDownload.ToString().ToLower()} "  // 断点续传
            "--allow-overwrite=true " // 允许复写文件
            "--auto-file-renaming=false "
            "--file-allocation={config.FileAllocation.ToString("G").ToLower()} " // 文件预分配, none prealloc
            "{headers}" + // header
            "";          // 命令行

        STARTUPINFO si;                // 启动信息
        PROCESS_INFORMATION pi;        // 进程头
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        // 创建子进程，并判断是否成功
        if (!CreateProcess(ariaExefilename.toStdWString().c_str(),       // 使用命令行
            startAgr.toStdWString().c_str(),
            NULL,             // 不继承进程句柄
            NULL,             // 不继承线程句柄
            FALSE,            // 不继承句柄
            0,                // 没有创建标志
            NULL,             // 使用父进程环境变量
            NULL,             // 使用父进程目录作为当前目录
            &si,              // STARTUPINFO 结构
            &pi)              // PROCESS_INFORMATION 保存相关信息
            )
        {
            // 创建失败
            printf("CreateProcess failed (%d).\n", GetLastError());
            return false;
        }

        return true;
    });
}
