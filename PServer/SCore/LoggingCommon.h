#pragma once

constexpr const char* DEFAULT_LOG_PATH = "./Log/";			// 로그 루트 폴더
constexpr const char* DEFAULT_LOG_INIT_PATH = "./";			// 시작시 지정될 로그 폴더. YYMMDD format
constexpr const char* DEFAULT_SERVER_NAME = "Server";		// 서버명 (Login, Game 등)
constexpr int DEFAULT_SERVER_NUMBER = 10000;				// 서버번호

constexpr size_t MAX_LOG_FILE_SIZE = 1024 * 1024 * 10;	// 단일 파일 최대 10M
constexpr size_t MAX_LOG_STRING_SIZE = 8192;				// 로그 메시지 하나당 최대 글자수
constexpr size_t VARIABLE_FORMAT_BUFFER_SIZE = MAX_LOG_STRING_SIZE * 2;	// 가변인자 버퍼 크기

// Format : [YYYY-MM-DD HH:mm:ss.xxx] LogMessage...
constexpr const wchar_t* LOG_MESSAGE_FORMAT = L"[%04d-%02d-%02d %02d:%02d:%02d.%03d] %s";

constexpr long long LOG_THREAD_TICK_MS = 25;

enum class ELogLevel : int
{
    None = 0,

    Info,
    Warning,
    Error,

    System,			// 서버 가동 등 시스템적인 요소, View Only
    Debug,			// Debug, View Only

    Max,
};

