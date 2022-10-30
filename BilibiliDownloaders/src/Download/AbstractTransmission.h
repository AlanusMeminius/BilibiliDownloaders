#pragma once

#include <thread>
#include <condition_variable>
#include <mutex>
#include <map>

class AbstractTransmission
{
public:
    virtual ~AbstractTransmission() = default;
    virtual void start() = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void stop() = 0;
};


class AbstractObserver
{
public:
    virtual ~AbstractObserver() = default;
    virtual void update(double procent) = 0;
};

class AbstractSubject : public AbstractObserver
{
public:
    virtual ~AbstractSubject() = default;
    virtual void AddObserver(AbstractObserver* observer) = 0;
    virtual void RemoveObserver(AbstractObserver* observer) = 0;
};
