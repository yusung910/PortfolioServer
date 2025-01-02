#pragma once
#include "SCoreAPI.h"
#include <string>
#include <unordered_map>

constexpr const char* DEFAULT_CTXT_TYPE = "application/x-www-form-urlencoded";
constexpr const char* JSON_CTXT_TYPE = "application/JSON";

constexpr int HTTP_PORT = 80;
constexpr int HTTPS_PORT = 443;

class HttpPO;

class SCoreAPI Http
{
private:
    HttpPO* m_pHttp = nullptr;

public:
    Http();
    ~Http();


private:
    bool _HttpReq(std::string& _str
        , const std::string& _url
        , const std::string& _subUrl
        , const int& _port
        , const std::string _datas
        , bool _isHttps
        , bool _isPost);
    std::string _MakeHeader();


};

