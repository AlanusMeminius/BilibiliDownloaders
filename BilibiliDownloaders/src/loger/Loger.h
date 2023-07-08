#pragma once

#include <string>

class Logger
{
public:
	static Logger& GetInstance();
	void InitLog();

private:
	Logger();
	~Logger();

	// 单例禁止
	Logger(const Logger& other) = delete;
	Logger& operator=(const Logger& other) = delete;
    Logger(Logger&& other) = delete;
    Logger& operator=(Logger&& other) = delete;

	// 注册logger和设置
	void RegisterLogger(const std::string& logName);
	void SetLog();

	static Logger m_logger;
};
