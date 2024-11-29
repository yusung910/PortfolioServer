#pragma once
#include <sqlext.h>

constexpr size_t PROPERTY_MAX_FIELD_SIZE = 8192;
//constexpr size_t DEFAULT_DATABASE_TIMEOUT = 5;		// 단위는 확인 필요
constexpr int MAX_CONNECT_TRY_COUNT = 20;			// 최대 재접속 시도 횟수

//https://jjseol.blogspot.com/2017/03/c-odbc-dbms-how-to-connect-to-dbms-with.html
class DBWorkerPO
{
private:
	
	//DB Connection Info
	SQLHENV m_sqlHandle;
	SQLHDBC m_sqlHDBC;
	SQLHSTMT m_sqlHstmt;
	SQLRETURN m_sqlRetCode;

	// For specific error

	SQLSMALLINT m_sqlnLength;
	SQLINTEGER m_sqlnRec = 0, m_sqlnNative;
	SQLCHAR m_sqlcState[7], m_sqlcMsg[256];



public:
	DBWorkerPO() = default;
	~DBWorkerPO();

	void SetDBConfig();
};

