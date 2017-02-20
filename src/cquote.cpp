// cquote.cpp

#include "cquote.hpp"

#include <stdexcept>
#include <stdio.h>
#include <wchar.h>
#include <locale>

std::wstring widen(std::string const& s, std::locale loc)
{
    std::char_traits<wchar_t>::state_type state = { 0 };

    typedef std::codecvt<wchar_t, char, std::char_traits<wchar_t>::state_type> ConverterFacet;

    ConverterFacet const& converter(std::use_facet<ConverterFacet>(loc));

    char const* nextToRead = s.data();
    wchar_t buffer[BUFSIZ];
    wchar_t* nextToWrite;
    std::codecvt_base::result result;
    std::wstring wresult;

    while ((result = converter.in(state,
                                  nextToRead, s.data() + s.size(), nextToRead,
                                  buffer, buffer + sizeof(buffer) / sizeof(*buffer), nextToWrite))
           == std::codecvt_base::partial)
    {
        wresult.append(buffer, nextToWrite);
    }

    if (result == std::codecvt_base::error) {
        throw std::runtime_error("Encoding error");
    }
    wresult.append(buffer, nextToWrite);
    return wresult;
}

std::wstring cquote(std::string const& src, bool useLocale)
{
    std::wstring result;

    if (useLocale) {
        std::wstring wsrc = widen(src, std::locale());
        for (size_t i = 0; i < wsrc.size(); ++i) {
            if (iswprint((wint_t) wsrc[i]) && wsrc[i] != L'\\' && wsrc[i] != L'\"'
                && wsrc[i] != L'\'' && wsrc[i] != L'\?')
            {
                result += wsrc[i];
            } else {
                switch (wsrc[i]) {
                case L'\a': result += L"\\a"; break;
                case L'\b': result += L"\\b"; break;
                case L'\f': result += L"\\f"; break;
                case L'\n': result += L"\\n"; break;
                case L'\r': result += L"\\r"; break;
                case L'\t': result += L"\\t"; break;
                case L'\v': result += L"\\v"; break;
                case L'\'': result += L"\\'"; break;
                case L'\"': result += L"\\\""; break;
                case L'\?': result += L"\\?"; break;
                case L'\\': result += L"\\\\"; break;
                default:
                    wchar_t buffer[10];
                    swprintf(buffer, sizeof(buffer)/sizeof*buffer, L"\\0x%04X", (wint_t) wsrc[i]);
                    result += buffer;
                    break;
                }
            }
        }
    } else {
        for (size_t i = 0; i < src.size(); ++i) {
            if (isprint((int) src[i]) && src[i] != '\\' && src[i] != '\"'
                && src[i] != '\'' && src[i] != '\?')
            {
                result += wchar_t(src[i]);
            } else {
                switch (src[i]) {
                case '\a': result += L"\\a"; break;
                case '\b': result += L"\\b"; break;
                case '\f': result += L"\\f"; break;
                case '\n': result += L"\\n"; break;
                case '\r': result += L"\\r"; break;
                case '\t': result += L"\\t"; break;
                case '\v': result += L"\\v"; break;
                case '\'': result += L"\\'"; break;
                case '\"': result += L"\\\""; break;
                case '\?': result += L"\\?"; break;
                case '\\': result += L"\\\\"; break;
                default:
                    wchar_t buffer[10];
                    swprintf(buffer, sizeof(buffer)/sizeof*buffer, L"\\0x%02X", (unsigned char)src[i]);
                    result += buffer;
                    break;
                }
            }
        }
    }
    return result;
}
