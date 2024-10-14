#pragma once
#include "LoggingManager.h"
#include "LoggingCommon.h"

// New Logging Macro
#define VIEW_INFO(fmt, ...)                   LoggingManager::GetInst().Log(__FUNCTION__, ELogLevel::Info, true, false, fmt, __VA_ARGS__)
#define WRITE_INFO(fmt, ...)                  LoggingManager::GetInst().Log(__FUNCTION__, ELogLevel::Info, false, true, fmt, __VA_ARGS__)
#define VIEW_WRITE_INFO(fmt, ...)             LoggingManager::GetInst().Log(__FUNCTION__, ELogLevel::Info, true, true, fmt, __VA_ARGS__)

#define VIEW_WARNING(fmt, ...)                LoggingManager::GetInst().Log(__FUNCTION__, ELogLevel::Warning, true, false, fmt, __VA_ARGS__)
#define WRITE_WARNING(fmt, ...)               LoggingManager::GetInst().Log(__FUNCTION__, ELogLevel::Warning, false, true, fmt, __VA_ARGS__)
#define VIEW_WRITE_WARNING(fmt, ...)          LoggingManager::GetInst().Log(__FUNCTION__, ELogLevel::Warning, true, true, fmt, __VA_ARGS__)
#define VIEW_WRITE_WARNING_LINE(fmt, ...)     LoggingManager::GetInst().Log(__FUNCTION__, __LINE__, ELogLevel::Warning, true, true, fmt, __VA_ARGS__)

#define VIEW_ERROR(fmt, ...)                  LoggingManager::GetInst().Log(__FUNCTION__, ELogLevel::Error, true, false, fmt, __VA_ARGS__)
#define WRITE_ERROR(fmt, ...)                 LoggingManager::GetInst().Log(__FUNCTION__, ELogLevel::Error, false, true, fmt, __VA_ARGS__)
#define VIEW_WRITE_ERROR(fmt, ...)            LoggingManager::GetInst().Log(__FUNCTION__, ELogLevel::Error, true, true, fmt, __VA_ARGS__)
#define VIEW_WRITE_ERROR_LINE(fmt, ...)       LoggingManager::GetInst().Log(__FUNCTION__, __LINE__, ELogLevel::Error, true, true, fmt, __VA_ARGS__)

#define VIEW_SYSTEM(fmt, ...)                 LoggingManager::GetInst().Log(__FUNCTION__, ELogLevel::System, true, false, fmt, __VA_ARGS__)
#define VIEW_DEBUG(fmt, ...)                  LoggingManager::GetInst().Log(__FUNCTION__, ELogLevel::Debug, true, false, fmt, __VA_ARGS__)
#define VIEW_SYSTEM_FORCE(fmt, ...)           LoggingManager::GetInst().ViewForce(__FUNCTION__, ELogLevel::System, fmt, __VA_ARGS__)
#define VIEW_ERROR_FORCE(fmt, ...)            LoggingManager::GetInst().ViewForce(__FUNCTION__, ELogLevel::Error, fmt, __VA_ARGS__)


#define VIEW_INFO_N(fmt, ...)                 LoggingManager::GetInst()Log(nullptr, ELogLevel::Info, true, false, fmt, __VA_ARGS__)
#define WRITE_INFO_N(fmt, ...)                LoggingManager::GetInst()Log(nullptr, ELogLevel::Info, false, true, fmt, __VA_ARGS__)
#define VIEW_WRITE_INFO_N(fmt, ...)           LoggingManager::GetInst()Log(nullptr, ELogLevel::Info, true, true, fmt, __VA_ARGS__)

#define VIEW_WARNING_N(fmt, ...)              LoggingManager::GetInst()Log(nullptr, ELogLevel::Warning, true, false, fmt, __VA_ARGS__)
#define WRITE_WARNING_N(fmt, ...)             LoggingManager::GetInst()Log(nullptr, ELogLevel::Warning, false, true, fmt, __VA_ARGS__)
#define VIEW_WRITE_WARNING_N(fmt, ...)        LoggingManager::GetInst()Log(nullptr, ELogLevel::Warning, true, true, fmt, __VA_ARGS__)

#define VIEW_ERROR_N(fmt, ...)                LoggingManager::GetInst()Log(nullptr, ELogLevel::Error, true, false, fmt, __VA_ARGS__)
#define WRITE_ERROR_N(fmt, ...)               LoggingManager::GetInst()Log(nullptr, ELogLevel::Error, false, true, fmt, __VA_ARGS__)
#define VIEW_WRITE_ERROR_N(fmt, ...)          LoggingManager::GetInst()Log(nullptr, ELogLevel::Error, true, true, fmt, __VA_ARGS__)

#define VIEW_SYSTEM_N(fmt, ...)               LoggingManager::GetInst().Log(nullptr, ELogLevel::System, true, false, fmt, __VA_ARGS__)
#define VIEW_DEBUG_N(fmt, ...)                LoggingManager::GetInst().Log(nullptr, ELogLevel::Debug, true, false, fmt, __VA_ARGS__)
#define VIEW_SYSTEM_FORCE_N(fmt, ...)         LoggingManager::GetInst().ViewForce(nullptr, ELogLevel::System, fmt, __VA_ARGS__)
#define VIEW_ERROR_FORCE_N(fmt, ...)          LoggingManager::GetInst().ViewForce(nullptr, ELogLevel::Error, fmt, __VA_ARGS__)


// end

