#pragma once
#include <unordered_map>
#include <string>
class HttpPO
{
public:
	std::unordered_map<std::string, std::string> m_umList;
	std::string m_sCtxtType = "application/x-www-form-urlencoded";
	std::string m_sURL = "";
	int m_nPort = 80;

	std::string m_sSubUrl = "";
	std::string m_sCtxtData = "";

	bool m_bIsPost = false;
	bool m_bIsSecure = false;

public:
	HttpPO() = default;
	~HttpPO();
};

