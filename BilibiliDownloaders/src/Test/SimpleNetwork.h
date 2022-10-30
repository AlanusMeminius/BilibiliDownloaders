#pragma once

#include <QObject>
#include <QFile>


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
