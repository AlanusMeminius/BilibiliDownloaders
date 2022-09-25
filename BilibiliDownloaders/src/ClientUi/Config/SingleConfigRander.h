#pragma once

#include <memory>

class QSettings;

class SingleConfigRander 
{
public:
    SingleConfigRander();
    ~SingleConfigRander();

private:
    std::shared_ptr<QSettings> m_Setting;
};
