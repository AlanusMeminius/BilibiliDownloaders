#include <qrencode.h>

#include <QFile>

#include "LoginWidget.h"
#include "ui_LoginWidget.h"
#include "BiliApi/BilibiliClient.h"


LoginWidget::LoginWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWidget())
    , m_bIsrc(false)
    , m_bStop(false) 
    , m_thread(&LoginWidget::MonitorStatus, this)
    
{
    ui->setupUi(this);
    SetUi();

    SignalsAndSlots();

    GetLoginOrc();
}

LoginWidget::~LoginWidget()
{
    Stop();
    if (m_thread.joinable()) 
    {
        m_thread.join();
    }
    delete ui;
}

void LoginWidget::SetUi() 
{

}

void LoginWidget::SignalsAndSlots() 
{

}

void LoginWidget::GetLoginOrc() 
{
    auto        loginUrl = BiliApi::BilibiliClient::GetInstance().GetLoginUrl();
    std::string strLoginUrl = loginUrl.data.url;
    m_strOauthKey           = loginUrl.data.oauthKey;
    if (!m_strOauthKey.empty()) 
    {
        m_bIsrc = true;
        m_cv.notify_one();
    }
    QImage      image       = Qrcode(strLoginUrl);
    QPixmap     showPixmap  = QPixmap::fromImage(image).scaled(
             ui->labelOrc->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->labelOrc->setPixmap(showPixmap);
}

void LoginWidget::Stop() 
{
    m_bIsrc = true;
    m_bStop = true;
    m_cv.notify_all();
}

QImage LoginWidget::Qrcode(const std::string& content)
{
    unsigned char *pRGBData, *pSourceData, *pDestData;
    QRcode*        pQRC = nullptr;

    if (pQRC = QRcode_encodeString(content.c_str(), 0, QR_ECLEVEL_H, QR_MODE_8,
                                   1)) 
    {
        int unWidth     = pQRC->width;
        int unWidthAdjusted = unWidth * 8 * 3;
        if (unWidthAdjusted % 4) 
        {
            unWidthAdjusted = (unWidthAdjusted / 4 + 1) * 4;
        }
        int unDataBytes = unWidthAdjusted * unWidth * 8;
        pRGBData    = (unsigned char*)malloc(unDataBytes);
        memset(pRGBData, 0xff, unDataBytes);

        // Convert QrCode bits to bmp pixels
        pSourceData = pQRC->data;
        for (int y = 0; y < unWidth; y++) 
        {
            pDestData = pRGBData + unWidthAdjusted * y * 8;
            for (int x = 0; x < unWidth; x++) 
            {
                if (*pSourceData & 1) 
                {
                    for (int l = 0; l < 8; l++) 
                    {
                        for (int n = 0; n < 8; n++) 
                        {
                            //以下三行是设置三基色 三基色设置0x00
                            //则二维码颜色就是黑色的
                            *(pDestData + n * 3 + unWidthAdjusted * l) = 0xff;
                            *(pDestData + 1 + n * 3 + unWidthAdjusted * l) =
                                0x00;
                            *(pDestData + 2 + n * 3 + unWidthAdjusted * l) =
                                0x00;
                        }
                    }
                }
                pDestData += 3 * 8;
                pSourceData++;
            }
        }

        // output the bmp file
        QImage image(pRGBData, unWidth * 8, unWidth * 8, unWidthAdjusted,
                     QImage::Format_BGR888);
        image = image.copy();

        // Free data
        free(pRGBData);
        QRcode_free(pQRC);

        return image;
    }

    return {};
}

bool LoginWidget::WriteCookie(const QString& url, const QString& filePath)
{
    if (!url.contains("?")) {
        return false;
    }

    auto urlSplit = url.split("?");
    if (urlSplit.size() < 2) 
    {
        return false;
    }

    auto paramsSplit = urlSplit[1].split("&");
    if (paramsSplit.size() < 2)
    {
        return false;
    }

    QString writeData;
    for (const auto& param : paramsSplit) 
    {
        if (param.startsWith("Expires") || param.startsWith("gourl")) 
        {
            continue;
        }

        if (param.contains("=")) 
        {
            writeData += param + ";";
        }
    }


    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) 
    {
        file.write(writeData.toLocal8Bit());
        file.close();
        return true;
    }

    return false;
}


void LoginWidget::MonitorStatus() 
{
    while (true) 
    {
        if (m_bStop) 
        {
            return;
        }

        std::unique_lock<SNullMutex> lk(m_nullMutex);
        m_cv.wait(lk, [&]() { return !m_strOauthKey.empty(); });

        auto loginUrlStatus = BiliApi::BilibiliClient::GetInstance().GetLoginStatus(m_strOauthKey);
        auto json = nlohmann::json::parse(loginUrlStatus.toString());
        if (!json.is_null() && json.contains("code")) 
        {
            BiliApi::LoginStatus loginStatus1(json);
            int nCode = loginStatus1.code;
            switch (nCode) 
            {
            case -1: // 没有这个oauthKey
            {
                m_strOauthKey.clear();
                LoginOrcAgain();
                break;
            }
            case -2: 
            {
                // 不匹配的oauthKey，超时或已确认的oauthKey
                LoginOrcAgain();
                break;
            }
            case -5: 
            {
                // 已扫码，未确认
                QrcScaned();
                break;
            }
            case 0: 
            {
                // 已扫码，已确认
                //BiliApi::LoginStatusReady loginStatusReady(json);

                emit loginStatus(true);
                m_bStop = true;
                break;
            }
            default: 
                break;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
