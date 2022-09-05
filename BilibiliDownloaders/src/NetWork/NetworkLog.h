#pragma once
#pragma execution_character_set("utf-8")

#include <spdlog/spdlog.h>
#include <spdlog/common.h>

#define NetWork_LOG_TRACE(...) SPDLOG_LOGGER_CALL(spdlog::get("Network"), spdlog::level::trace, __VA_ARGS__)
#define NetWork_LOG_DEBUG(...) SPDLOG_LOGGER_CALL(spdlog::get("Network"), spdlog::level::debug, __VA_ARGS__)
#define NetWork_LOG_INFO(...) SPDLOG_LOGGER_CALL(spdlog::get("Network"), spdlog::level::info, __VA_ARGS__)
#define NetWork_LOG_WARN(...) SPDLOG_LOGGER_CALL(spdlog::get("Network"), spdlog::level::warn, __VA_ARGS__)
#define NetWork_LOG_ERROR(...) SPDLOG_LOGGER_CALL(spdlog::get("Network"), spdlog::level::err, __VA_ARGS__)
#define NetWork_LOG_CRITICAL(...) SPDLOG_LOGGER_CALL(spdlog::get("Network"), spdlog::level::critical, __VA_ARGS__)
#define NetWork_LOG_OFF(...) SPDLOG_LOGGER_CALL(spdlog::get("Network"), spdlog::level::off, __VA_ARGS__)

