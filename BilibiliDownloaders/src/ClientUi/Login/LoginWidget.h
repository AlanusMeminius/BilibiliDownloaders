#pragma once

#include <QWidget>
#include <thread>
#include <condition_variable>

namespace Ui { class LoginWidget; };

struct SNullMutex
{
    void lock() const {}
    void unlock() const {}
};

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();

    void SetUi();
    void SignalsAndSlots();

    void GetLoginOrc();
    void Stop();

    static QImage Qrcode(const std::string& content);
    static bool   WriteCookie(const QString& url, const QString& filePath);

    void MonitorStatus();
signals:
    void loginStatus(bool bSuccess);
    void LoginOrcAgain();
    void QrcScaned();

private:
    Ui::LoginWidget* ui;
    std::string                 m_strOauthKey;
    volatile bool               m_bIsrc;
    volatile bool               m_bStop;
    SNullMutex       m_nullMutex;
    std::condition_variable_any m_cv;
    std::thread                 m_thread;
};
