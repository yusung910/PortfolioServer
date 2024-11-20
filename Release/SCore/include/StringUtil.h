#pragma once
#include "SCoreAPI.h"
#include <string>

class SCoreAPI StringUtil
{
public:
    static std::string ToLower(const std::string& _str);
    static std::string ToUpper(const std::string& _str);

    static std::wstring ToLower(const std::wstring& _str);
    static std::wstring ToUpper(const std::wstring& _str);

    static std::u8string ToLower(const std::u8string& _str);
    static std::u8string ToUpper(const std::u8string& _str);


    static std::string ToMultiByte(const std::wstring& _str);
    static std::string ToMultiByte(const std::u8string& _str);

    static std::wstring ToWideChar(const std::string& _str);
    static std::wstring ToWideChar(const std::u8string& _str);

    static std::u8string ToUTF8(const std::string& _str);
    static std::u8string ToUTF8(const std::wstring& _str);

    static std::string UTF8_JsonSTR(const std::u8string& _str);


    static std::string WSTR_UTF8(const std::wstring& _src);
    static std::wstring UTF8_WSTR(const std::string& _src);


    static int ToInt(const std::string& _str);
    static unsigned long ToUnsignedLong(const std::string& _str);

    static std::string ReplaceAll(const std::string& _src, const std::string& _pattern, const std::string& _replacement);
    static std::wstring ReplaceAll(const std::wstring& _src, const std::wstring& _pattern, const std::wstring& _replacement);
    static std::u8string ReplaceAll(const std::u8string& _src, const std::u8string& _pattern, const std::u8string& _replacement);

    static std::string STR_UTF8(const std::string& _src);
    static std::u32string UTF8_UTF32(const std::string& _src);

    static std::string MakeNameServerID(const std::string& _strName, int _ServerID);
    static int StringPraseServerID(const std::string& _strName);

    static constexpr unsigned int Hash(const char* _str)
    {
        return _str[0] ? static_cast<unsigned int>(_str[0]) + 0xEDB8832Full * Hash(_str + 1) : 8603;
    }

    static constexpr unsigned int Hash(const std::string& _str)
    {
        return _str.c_str()[0] ? static_cast<unsigned int>(_str.c_str()[0]) + 0xEDB8832Full * Hash(_str.c_str() + 1) : 8603;
    }

    template<typename ... Args>
    static std::wstring WStringFormat(const std::wstring& _format, Args ... _args)
    {
        std::wstring s;
        s.resize((size_t)(_scwprintf(_format.c_str(), _args ...)) + 1);
        s.resize(_snwprintf_s(s.data(), s.capacity(), _TRUNCATE, _format.c_str(), _args ...));

        return s;
    }

private:
    StringUtil() = delete;
    ~StringUtil() = delete;
};

