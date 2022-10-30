#pragma once

#include "AbstractTransmission.h"

class AriaDownloader  : public AbstractTransmission
{
public:
    AriaDownloader();
    ~AriaDownloader();

    void start() override;
    void pause() override;
    void resume() override;
    void stop() override;
};

class TransmissionControl
{
public:
    ~TransmissionControl() = default;

    void statusControl();

private:
    std::condition_variable m_cv;
    std::thread m_thread;
    std::mutex m_mutex;
};
