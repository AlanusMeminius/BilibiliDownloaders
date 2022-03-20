#pragma once

#include <QObject>

#include <chrono>
#include <thread>
#include <future>

class AriaServer : public QObject
{
    Q_OBJECT

public:
    AriaServer(QObject *parent);
    ~AriaServer();
    void StartServerAsync();
};
