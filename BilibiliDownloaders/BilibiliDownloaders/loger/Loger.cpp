#include "Loger.h"

#include <spdlog/spdlog.h>

Logger& Logger::GetInstance()
{
    static Logger logger;
    return logger;
}

Logger::Logger()
{
    m_logFile = std::move(spdlog::basic_logger_mt<spdlog::async_factory>("async_file_logger", "logs/Loggers.txt"));
}

Logger::~Logger()
{
    spdlog::shutdown();
}
