#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/common.h>

#include <utility>

class Logger
{
public:
	static Logger& GetInstance();
private:
	Logger();
	~Logger();

public:
	std::shared_ptr<spdlog::logger> m_logFile;
};

// 对外日志接口
template<typename... Args>
void LogInfo(spdlog::format_string_t<Args...> fmt, Args &&... args)
{
	Logger::GetInstance().m_logFile->info(fmt, std::forward<Args>(args)...);
}

template<typename... Args>
void LogWarn(spdlog::format_string_t<Args...> fmt, Args &&... args)
{
	Logger::GetInstance().m_logFile->warn(fmt, std::forward<Args>(args)...);
}

template<typename... Args>
void LogError(spdlog::format_string_t<Args...> fmt, Args &&... args)
{
	Logger::GetInstance().m_logFile->error(fmt, std::forward<Args>(args)...);
}

template<typename... Args>
void LogDebug(spdlog::format_string_t<Args...> fmt, Args &&... args)
{
	Logger::GetInstance().m_logFile->debug(fmt, std::forward<Args>(args)...);
}