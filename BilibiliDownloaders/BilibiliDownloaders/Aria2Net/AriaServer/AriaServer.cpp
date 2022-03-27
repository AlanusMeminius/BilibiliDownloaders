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
            //"--max-concurrent-downloads={config.MaxConcurrentDownloads} " // ���ͬʱ������(������)
            //"--max-connection-per-server={config.MaxConnectionPerServer} " // ͬ������������
            //"--split={config.Split} "  // ���ļ�����߳���
            //                            //$"--max-tries={config.MaxTries} retry-wait=3 " + // ������������
            //"--min-split-size={config.MinSplitSize}M "  // ��С�ļ���Ƭ��С, �����߳�������ȡ�����ֳܷ�����Ƭ, ����С�ļ���Ҫ
            //"--max-overall-download-limit={config.MaxOverallDownloadLimit} "  // �����ٶ�����
            //"--max-download-limit={config.MaxDownloadLimit} "  // ���ص��ļ��ٶ�����
            //"--max-overall-upload-limit={config.MaxOverallUploadLimit} "  // �ϴ��ٶ�����
            //"--max-upload-limit={config.MaxUploadLimit} "  // �ϴ����ļ��ٶ�����
            //"--continue={config.ContinueDownload.ToString().ToLower()} "  // �ϵ�����
            //"--allow-overwrite=true " // ����д�ļ�
            //"--auto-file-renaming=false "
            //"--file-allocation={config.FileAllocation.ToString("G").ToLower()} " // �ļ�Ԥ����, none prealloc
            //"{headers}"  // header
            //"";          // ������

        STARTUPINFO si;                // ������Ϣ
        PROCESS_INFORMATION pi;        // ����ͷ
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        // �����ӽ��̣����ж��Ƿ�ɹ�
        if (!::CreateProcess((LPWSTR)ariaExeWFilename.c_str(),              // ����������
                             (LPWSTR)startWAgr.c_str(),                     // ������
                             NULL,                                          // ���̳н��̾��
                             NULL,                                          // ���̳��߳̾��
                             FALSE,                                         // ���̳о��
                             0,                                             // û�д�����־
                             NULL,                                          // ʹ�ø����̻�������
                             NULL,                                          // ʹ�ø�����Ŀ¼��Ϊ��ǰĿ¼
                             &si,                                           // STARTUPINFO �ṹ
                             &pi))                                          // PROCESS_INFORMATION ���������Ϣ
        {
            // ����ʧ��
            qDebug() << QString("create aria2c.exe failed, error code: %1").arg(::GetLastError());
            return false;
        }

        return true;
    });
}
