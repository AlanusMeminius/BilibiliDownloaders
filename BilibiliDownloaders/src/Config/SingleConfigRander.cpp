#include <QApplication>
#include <QFile>
#include <QSettings>

#include "SingleConfigRander.h"

SingleConfigRander& SingleConfigRander::getInstance()
{
    static SingleConfigRander configRander;
    return configRander;
}

SingleConfigRander::SingleConfigRander()
    : m_configPath(QApplication::applicationDirPath() + "/Config/config.ini")
{
    init();
}

SingleConfigRander::~SingleConfigRander()
{
}

void SingleConfigRander::init()
{
    initConfig();
}

void SingleConfigRander::initConfig()
{
    if (QFile::exists(m_configPath)) 
    {
        m_setting = std::make_shared<QSettings>(m_configPath, QSettings::IniFormat);
        m_setting->setIniCodec("GB2312");
    }
    else 
    {
        m_setting = std::make_shared<QSettings>(m_configPath, QSettings::IniFormat);
        m_setting->setIniCodec("GB2312");

        m_setting->beginGroup("Test");
        m_setting->setValue("Test", "123");
    }
}
