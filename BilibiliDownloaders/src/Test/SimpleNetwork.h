#pragma once

#include <QObject>
#include <QFile>

#include <qrencode.h>
#include <iostream>
#include <Windows.h>

class QNetworkAccessManager;
class SimpleNetwork : public QObject
{
    Q_OBJECT

public:
    SimpleNetwork(QObject* parent = nullptr);
    ~SimpleNetwork();

private:
    QNetworkAccessManager* m_networkAccessManager;
    QFile m_videoFile;
    QFile m_audeoFile;
};


QImage qr_code(std::string _src_str, std::string _dst_bmp_path);