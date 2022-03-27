#pragma once

#include <QObject>

class QNetworkAccessManager;
class SimpleNetwork : public QObject
{
    Q_OBJECT

public:
    SimpleNetwork(QObject* parent = nullptr);
    ~SimpleNetwork();

private:
    QNetworkAccessManager* m_networkAccessManager;
};
