#pragma once

#include <memory>
#include <QString>

class QSettings;

class SingleConfigRander 
{
public:
    static SingleConfigRander& getInstance();

private:
    SingleConfigRander();
    ~SingleConfigRander();

    SingleConfigRander(const SingleConfigRander& other) = delete;
    SingleConfigRander& operator=(const SingleConfigRander& other) = delete;
    SingleConfigRander(SingleConfigRander&& other)             = delete;
    SingleConfigRander& operator=(SingleConfigRander&& other) = delete;

    void init();
    void initConfig();

    std::shared_ptr<QSettings> m_setting;
    QString                    m_configPath;
};
