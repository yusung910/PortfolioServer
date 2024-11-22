#pragma once

constexpr const char* DEFAULT_LOG_PATH = "./Log/";			// �α� ��Ʈ ����
constexpr const char* DEFAULT_LOG_INIT_PATH = "./";			// ���۽� ������ �α� ����. YYMMDD format
constexpr const char* DEFAULT_SERVER_NAME = "Server";		// ������ (Login, Game ��)
constexpr int DEFAULT_SERVER_NUMBER = 10000;				// ������ȣ

constexpr size_t MAX_LOG_FILE_SIZE = 1024 * 1024 * 10;	// ���� ���� �ִ� 10M
constexpr size_t MAX_LOG_STRING_SIZE = 8192;				// �α� �޽��� �ϳ��� �ִ� ���ڼ�
constexpr size_t VARIABLE_FORMAT_BUFFER_SIZE = MAX_LOG_STRING_SIZE * 2;	// �������� ���� ũ��

// Format : [YYYY-MM-DD HH:mm:ss.xxx] LogMessage...
constexpr const wchar_t* LOG_MESSAGE_FORMAT = L"[%04d-%02d-%02d %02d:%02d:%02d.%03d] %s";

constexpr long long LOG_THREAD_TICK_MS = 25;

enum class ELogLevel : int
{
    None = 0,

    Info,
    Warning,
    Error,

    System,			// ���� ���� �� �ý������� ���, View Only
    Debug,			// Debug, View Only

    Max,
};

