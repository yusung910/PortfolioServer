#include "stdafx.hxx"
#include "StringUtil.h"
#include "ConvertUTF.hxx"
#include <algorithm>
#include <vector>

std::string StringUtil::ToLower(const std::string& _str)
{
    std::string out;
    out.assign(_str.begin(), _str.end());
    std::transform(out.begin(), out.end(), out.begin(), [](char c) {return (char)std::tolower(c); });
    return out;
}

std::string StringUtil::ToUpper(const std::string& _str)
{
    std::string out;
    out.assign(_str.begin(), _str.end());
    std::transform(out.begin(), out.end(), out.begin(), [](char c) {return (char)std::toupper(c); });
    return out;
}

std::wstring StringUtil::ToLower(const std::wstring& _str)
{
    std::wstring out;
    out.assign(_str.begin(), _str.end());
    std::transform(out.begin(), out.end(), out.begin(), [](wchar_t c) {return (wchar_t)std::tolower(c); });
    return out;
}

std::wstring StringUtil::ToUpper(const std::wstring& _str)
{
    std::wstring out;
    out.assign(_str.begin(), _str.end());
    std::transform(out.begin(), out.end(), out.begin(), [](wchar_t c) {return (wchar_t)std::toupper(c); });
    return out;
}

std::u8string StringUtil::ToLower(const std::u8string& _str)
{
    std::u8string out;
    out.assign(_str.begin(), _str.end());
    std::transform(out.begin(), out.end(), out.begin(), [](char8_t c) {return (char8_t)std::tolower(c); });
    return out;
}

std::u8string StringUtil::ToUpper(const std::u8string& _str)
{
    std::u8string out;
    out.assign(_str.begin(), _str.end());
    std::transform(out.begin(), out.end(), out.begin(), [](char8_t c) {return (char8_t)std::toupper(c); });
    return out;
}

std::string StringUtil::ToMultiByte(const std::wstring& _str)
{
    char buffer[4096] = {};
    auto len = ::WideCharToMultiByte(CP_ACP, 0, _str.c_str(), -1, buffer, sizeof(buffer), nullptr, nullptr);
    if (len == 0)
        return "";
    return buffer;
}

std::string StringUtil::ToMultiByte(const std::u8string& _str)
{
    wchar_t buffer[4096] = {};
    auto len = ::MultiByteToWideChar(CP_UTF8, 0, (char*)_str.c_str(), -1, buffer, sizeof(buffer) / sizeof(wchar_t));
    if (len == 0)
        return "";

    char mbbuffer[4096] = {};
    len = ::WideCharToMultiByte(CP_ACP, 0, buffer, -1, mbbuffer, sizeof(mbbuffer), nullptr, nullptr);
    if (len == 0)
        return "";
    return mbbuffer;
}

std::wstring StringUtil::ToWideChar(const std::string& _str)
{
    wchar_t buffer[4096] = {};
    auto len = ::MultiByteToWideChar(CP_ACP, 0, _str.c_str(), -1, buffer, sizeof(buffer) / sizeof(wchar_t));
    if (len == 0)
        return L"";

    return buffer;
}

std::wstring StringUtil::ToWideChar(const std::u8string& _str)
{
    wchar_t buffer[4096] = {};
    auto len = ::MultiByteToWideChar(CP_UTF8, 0, (char*)_str.c_str(), -1, buffer, sizeof(buffer) / sizeof(wchar_t));
    if (len == 0)
        return L"";

    return buffer;
}


std::u8string StringUtil::ToUTF8(const std::string& _str)
{
    wchar_t buffer[4096] = {};
    auto len = ::MultiByteToWideChar(CP_ACP, 0, _str.c_str(), -1, buffer, sizeof(buffer) / sizeof(wchar_t));
    if (len == 0)
        return u8"";

    char8_t u8buffer[4096] = {};
    len = ::WideCharToMultiByte(CP_UTF8, 0, buffer, -1, (char*)u8buffer, sizeof(u8buffer), nullptr, nullptr);
    if (len == 0)
        return u8"";

    return u8buffer;
}

std::u8string StringUtil::ToUTF8(const std::wstring& _str)
{
    char8_t buffer[4096] = {};
    auto len = ::WideCharToMultiByte(CP_UTF8, 0, _str.c_str(), -1, (char*)buffer, sizeof(buffer), nullptr, nullptr);
    if (len == 0)
        return u8"";
    return buffer;
}

std::string StringUtil::UTF8_JsonSTR(const std::u8string& _str)
{
    //u8string을 json에 넣을 때 
    //DB가 nvarchar로 json의 u8string을 읽어 깨지는 현상이 발생
    //wstring으로 변환 후 다시 u8string으로 보내면 해결 
    return WSTR_UTF8(ToWideChar(_str));
}


std::string StringUtil::WSTR_UTF8(const std::wstring& _src)
{
    std::string ret;
    // NOTE : Src값이 null 또는 size값이 0 인 경우 변환 과정없이 리턴 처리
    // 로직처리에 따라서 사이즈 검사를 해야할 경우에는 호출단에서 처리해야한다.
    if (_src.empty() == true)
        return ret;

    size_t retCapacity = _src.length() * 4;	// UTF32 size = 4
    ret.resize(retCapacity);

    UTF8* retBase = (UTF8*)&ret[0];

    UTF8* out = (UTF8*)retBase;
    const UTF16* in = (const UTF16*)_src.c_str();
    ConvertUTF16toUTF8(&in, in + _src.length(), &out, out + retCapacity, ConversionFlags::lenientConversion);

    size_t outLen = out - retBase;
    ret.resize(outLen);
    return ret;
}

std::wstring StringUtil::UTF8_WSTR(const std::string& _src)
{
    std::wstring ret;

    // NOTE : Src값이 null 또는 size값이 0 인 경우 변환 과정없이 리턴 처리
    // 로직처리에 따라서 사이즈 검사를 해야할 경우에는 호출단에서 처리해야한다.
    if (_src.empty() == true)
        return ret;

    size_t retCapacity = _src.length();
    ret.resize(retCapacity);

    UTF16* retBase = (UTF16*)&ret[0];

    UTF16* out = (UTF16*)retBase;
    const UTF8* in = (const UTF8*)_src.c_str();
    ConvertUTF8toUTF16(&in, in + _src.length(), &out, out + retCapacity, ConversionFlags::lenientConversion);

    size_t outLen = out - retBase;
    ret.resize(outLen);
    return ret;
}

int StringUtil::ToInt(const std::string& _str)
{
    if (true == _str.empty())
        return 0;

    try
    {
        return std::stoi(_str);
    }
    catch (...)
    {
        return 0;
    }
}

unsigned long StringUtil::ToUnsignedLong(const std::string& _str)
{
    if (true == _str.empty())
        return 0;

    try
    {
        return std::stoul(_str);
    }
    catch (...)
    {
        return 0;
    }
}

std::string StringUtil::ReplaceAll(const std::string& _src, const std::string& _pattern, const std::string& _replacement)
{
    std::string result = _src;
    std::string::size_type pos = 0;
    std::string::size_type offset = 0;
    while ((pos = result.find(_pattern, offset)) != std::string::npos)
    {
        result.replace(result.begin() + pos, result.begin() + pos + _pattern.size(), _replacement);
        offset = pos + _replacement.size();
    }
    return result;
}

std::wstring StringUtil::ReplaceAll(const std::wstring& _src, const std::wstring& _pattern, const std::wstring& _replacement)
{
    std::wstring result = _src;
    std::wstring::size_type pos = 0;
    std::wstring::size_type offset = 0;
    while ((pos = result.find(_pattern, offset)) != std::wstring::npos)
    {
        result.replace(result.begin() + pos, result.begin() + pos + _pattern.size(), _replacement);
        offset = pos + _replacement.size();
    }
    return result;
}

std::u8string StringUtil::ReplaceAll(const std::u8string& _src, const std::u8string& _pattern, const std::u8string& _replacement)
{
    std::u8string result = _src;
    std::u8string::size_type pos = 0;
    std::u8string::size_type offset = 0;
    while ((pos = result.find(_pattern, offset)) != std::u8string::npos)
    {
        result.replace(result.begin() + pos, result.begin() + pos + _pattern.size(), _replacement);
        offset = pos + _replacement.size();
    }
    return result;
}

std::string StringUtil::STR_UTF8(const std::string& _src)
{
    return WSTR_UTF8(ToWideChar(_src));
}

std::u32string StringUtil::UTF8_UTF32(const std::string& _src)
{
    std::u32string ret;

    if (_src.empty() == true)
        return ret;

    size_t retCapacity = _src.length();
    ret.resize(retCapacity);

    UTF32* retBase = (UTF32*)&ret[0];

    UTF32* out = (UTF32*)retBase;
    const UTF8* in = (const UTF8*)_src.c_str();
    ConvertUTF8toUTF32(&in, in + _src.length(), &out, out + retCapacity, ConversionFlags::lenientConversion);

    size_t outLen = out - retBase;
    ret.resize(outLen);
    return ret;
}

std::string StringUtil::MakeNameServerID(const std::string& _strName, int _ServerID)
{
    std::string strNameServerID = _strName;
    strNameServerID.append("_");
    strNameServerID.append(std::to_string(_ServerID));
    strNameServerID.append("_");

    return strNameServerID;
}

int StringUtil::StringPraseServerID(const std::string& _strName)
{
    int retValue = 0;

    std::vector<std::string> values;
    std::string delimiter("_");

    auto tmpStr = _strName;

    size_t pos = 0;
    while ((pos = tmpStr.find(delimiter)) != std::string::npos)
    {
        values.push_back(tmpStr.substr(0, pos));
        tmpStr.erase(0, pos + delimiter.length());
    }

    if (values.size() > 0)
    {
        retValue = stoi(values[1]);
    }

    return retValue;
}
