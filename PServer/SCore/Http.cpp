#include "stdafx.hxx"
#include "HttpPO.hxx"
#include "Http.h"
#include <WinInet.h>

#pragma comment(lib, "Wininet.lib")

Http::Http()
{
    m_pHttp = new HttpPO();

}

Http::~Http()
{
    SafeDelete(m_pHttp);
}

void Http::SetContextType(const std::string& _str)
{
    if (nullptr != m_pHttp)
        m_pHttp->m_sCtxtType = _str;
}

std::string Http::GetContextType() const
{
    if (nullptr == m_pHttp)
        return "";
    return m_pHttp->m_sCtxtType;
}

void Http::SetServerURL(const std::string& _str)
{
    if (nullptr == m_pHttp)
        return;
    if (true == _str.empty())
        return;

    m_pHttp->m_sURL.assign(_str.begin(), _str.end());
}

std::string Http::GetServerURL() const
{
    if (nullptr == m_pHttp)
        return "";
    return m_pHttp->m_sURL;
}

void Http::SetServerPort(const int& _port)
{
    if (nullptr == m_pHttp)
        return;

    if (_port <= 0
        || _port >= 0x10000)
        return;

    m_pHttp->m_nPort = _port;
}

int Http::GetServerPort() const
{
    if (nullptr == m_pHttp)
        return 0;

    return m_pHttp->m_nPort;
}

void Http::SetSubURL(const std::string& _str)
{
    if (nullptr != m_pHttp)
        m_pHttp->m_sSubUrl = _str;
}

std::string Http::GetSubURL() const
{
    if (nullptr == m_pHttp)
        return "";

    return m_pHttp->m_sSubUrl;
}

void Http::SetContextData(const std::string& _str)
{
    if (nullptr != m_pHttp)
        m_pHttp->m_sCtxtData = _str;
}

std::string Http::GetContextData() const
{
    if (nullptr == m_pHttp)
        return "";

    return m_pHttp->m_sCtxtData;
}

void Http::SetSecure(const bool& _isSecure)
{
    if (nullptr != m_pHttp)
        m_pHttp->m_bIsSecure = _isSecure;
}

bool Http::GetSecure() const
{
    if (nullptr == m_pHttp)
        return false;

    return m_pHttp->m_bIsSecure;
}

void Http::SetPost(const bool& _Post)
{
    if (nullptr != m_pHttp)
        m_pHttp->m_bIsPost = _Post;
}

bool Http::IsPost() const
{
    if (nullptr == m_pHttp)
        return false;
    return m_pHttp->m_bIsPost;
}

void Http::AddHeader(const std::string& _key, const std::string& _val)
{
    if (nullptr != m_pHttp)
        m_pHttp->m_umList.insert_or_assign(_key, _val);
}

std::string Http::GetHeader(const std::string& _key)
{
    if (nullptr == m_pHttp)
        return "";

    auto it = m_pHttp->m_umList.find(_key);
    if (it != m_pHttp->m_umList.end())
        return it->second;

    return std::string();
}


bool Http::RequestHTTP(std::string& _rslt)
{
    if (nullptr == m_pHttp)
        return false;
    return _HttpReq(_rslt, m_pHttp->m_sURL, m_pHttp->m_sSubUrl, m_pHttp->m_nPort, m_pHttp->m_sCtxtData, m_pHttp->m_bIsSecure, m_pHttp->m_bIsPost);
}

bool Http::RemoveHeader(const std::string& _key)
{
    if (nullptr == m_pHttp)
        return false;
    return m_pHttp->m_umList.erase(_key);
}

bool Http::_HttpReq(std::string& _rslt, const std::string& _url, const std::string& _subUrl, const int& _port, const std::string _datas, bool _isHttps, bool _isPost)
{
    if (nullptr == m_pHttp)
        return false;

    HINTERNET lConn = NULL;
    HINTERNET lSess = NULL;
    HINTERNET lReq = NULL;

    auto Release = [&](bool _ret = false) -> bool
        {
            if (NULL != lReq)
                InternetCloseHandle(lReq);
            if (NULL != lConn)
                InternetCloseHandle(lConn);
            if (NULL != lSess)
                InternetCloseHandle(lSess);

            lConn = NULL;
            lSess = NULL;
            lReq = NULL;

            return _ret;
        };

    //WORD
    INTERNET_PORT lConnPort = (INTERNET_PORT)_port;
    DWORD lDwFlag = 0;

    std::string lHeader = _MakeHeader();

    lSess = InternetOpen(L"Request", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, lDwFlag);

    if (NULL == lSess)
        return false;

    lConn = InternetConnectA(lSess, _url.c_str(), lConnPort, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL);

    if (NULL == lConn)
        return Release();

    //Connect Option
    lDwFlag = INTERNET_FLAG_RELOAD |
        INTERNET_FLAG_DONT_CACHE |
        INTERNET_FLAG_NO_COOKIES;

    //보안 프로토콜(https)
    if (true == _isHttps)
        lDwFlag |= INTERNET_FLAG_SECURE;

    if (true == _isPost)
    {
        lReq = HttpOpenRequestA(lConn, "POST", _subUrl.c_str(), "HTTP/1.1", NULL, 0, lDwFlag, 0);
    }
    else
    {
        if (true == _datas.empty())
        {
            lReq = HttpOpenRequestA(lConn, "GET", _subUrl.c_str(), "HTTP/1.1", NULL, 0, lDwFlag, 0);
        }
        else
        {
            lReq = HttpOpenRequestA(lConn, "GET", (std::string(_subUrl + "?") + _datas).c_str(), "HTTP/1.1", NULL, 0, lDwFlag, 0);
        }
    }

    if (NULL == lReq)
        return Release();

    if (true == m_pHttp->m_bIsSecure)
    {
#ifdef _DEBUG
        lDwFlag =
            SECURITY_FLAG_IGNORE_REVOCATION |
            SECURITY_FLAG_IGNORE_REDIRECT_TO_HTTP |
            SECURITY_FLAG_IGNORE_REDIRECT_TO_HTTPS |
            SECURITY_FLAG_IGNORE_UNKNOWN_CA |
            SECURITY_FLAG_IGNORE_CERT_DATE_INVALID |
            SECURITY_FLAG_IGNORE_CERT_CN_INVALID;
#else
        lDwFlag =
            SECURITY_FLAG_IGNORE_REVOCATION |
            SECURITY_FLAG_IGNORE_REDIRECT_TO_HTTP |
            SECURITY_FLAG_IGNORE_REDIRECT_TO_HTTPS;
#endif // _DEBUG
        if (FALSE == InternetSetOptionA(lReq, INTERNET_OPTION_SECURITY_FLAGS, &lDwFlag, sizeof(lDwFlag)))
            return Release();
}

    //Http 통신할 때 헤더 값이 비어 있고 찾은 경우 제거, 비어있지 않을경우 헤더값 교체
    //Http 관련 통신 할 때 사용되는 옵션
    //https://learn.microsoft.com/ko-kr/windows/win32/api/wininet/nf-wininet-httpaddrequestheadersa
    lDwFlag = HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD;
    if (FALSE == HttpAddRequestHeadersA(lReq, lHeader.c_str(), (DWORD)lHeader.length(), lDwFlag))
        return Release();

    if (true == _isPost)
    {
        if (FALSE == HttpSendRequestA(lReq, NULL, NULL, (LPVOID)_datas.c_str(), (DWORD)_datas.length()))
            return Release();
    }
    else
    {
        if (FALSE == HttpSendRequestA(lReq, NULL, NULL, NULL, 0))
            return Release();
    }

    _rslt.clear();

    // Maximum Transmission Unit 
    // https://skstp35.tistory.com/284
    // 데이터링크에서 하나의 프레임 또는 패킷에 담아 운반 가능한 최대 크기
    char lBuff[1420] = { 0, };
    DWORD lDwBytesRead = 0;
    DWORD lDwTotalRecvSize = 0;
    BOOL lRead = FALSE;

    do
    {
        lRead = InternetReadFile(lReq, lBuff, sizeof(lBuff), &lDwBytesRead);

        if (TRUE == lRead
            && lDwBytesRead > 0)
        {
            _rslt += std::string(lBuff, lDwBytesRead);
            lDwTotalRecvSize += lDwBytesRead;
        }
    } while (TRUE == lRead && lDwBytesRead > 0);

    return Release(true);
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