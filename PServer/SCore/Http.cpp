#include "stdafx.hxx"
#include "HttpPO.hxx"
#include "Http.h"


bool Http::_HttpReq(std::string& _str, const std::string& _url, const std::string& _subUrl, const int& _port, const std::string _datas, bool _isHttps, bool _isPost)
{
    return false;
}

std::string Http::_MakeHeader()
{
    if (nullptr == m_pHttp)
        return "";

    std::string lRetHeader;
    lRetHeader += "Content-type: " + m_pHttp->m_sCtxtType + "\r\n";
    if (true == m_pHttp->m_bIsPost)
    {
        lRetHeader += "Content-length: " + std::to_string(m_pHttp->m_sCtxtData.length()) + "\r\n";
    }

    for (auto& it : m_pHttp->m_umList)
    {
        lRetHeader += it.first + ": " + it.second + "\r\n";
    }
    lRetHeader += "\r\n\r\n";

    return lRetHeader;
}
