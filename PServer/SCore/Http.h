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

    //Setter, Getter
    void SetContextType(const std::string& _str = DEFAULT_CTXT_TYPE);
    std::string GetContextType() const;

    void SetServerURL(const std::string& _str);
    std::string GetServerURL() const;

    void SetServerPort(const int& _port);
    int GetServerPort() const;

    void SetSubURL(const std::string& _str);
    std::string GetSubURL() const;

    void SetContextData(const std::string& _str);
    std::string GetContextData() const;

    void SetSecure(const bool& _isSecure = true);
    bool GetSecure() const;

    void SetPost(const bool& _Post = true);
    bool IsPost() const;

    void AddHeader(const std::string& _key, const std::string& _val);
    std::string GetHeader(const std::string& _key);

    bool RequestHTTP(std::string& _rslt);
    bool RemoveHeader(const std::string& _key);


private:
    bool _HttpReq(std::string& _rslt
        , const std::string& _url
        , const std::string& _subUrl
        , const int& _port
        , const std::string _datas
        , bool _isHttps
        , bool _isPost);
    std::string _MakeHeader();
};

