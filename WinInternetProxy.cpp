#include "WinInternetProxy.h"
#include <Windows.h>
#include <wininet.h>
#pragma comment(lib,"wininet.lib")

std::string WinInternetProxy::query_http_version()
{
    INTERNET_VERSION_INFO _verison = { 0 };
    DWORD dwBufSize = sizeof(INTERNET_VERSION_INFO);
    InternetQueryOptionA(nullptr, INTERNET_OPTION_VERSION, &_verison, &dwBufSize);
    return std::to_string(_verison.dwMajorVersion) + "." + std::to_string(_verison.dwMinorVersion);
}

bool WinInternetProxy::query_proxy_option()
{
    // 通常查询4个标志位的信息就够使用了,我这里全部查询是让大家更明白该 WINAPI 如何使用.
    INTERNET_PER_CONN_OPTIONA _option[10];
    _option[0].dwOption = INTERNET_PER_CONN_FLAGS;// value 使用代理服务器 代理UI界面状态组合
    _option[1].dwOption = INTERNET_PER_CONN_PROXY_SERVER;// str 代理地址和端口
    _option[2].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;// str 代理过滤地址
    _option[3].dwOption = INTERNET_PER_CONN_AUTOCONFIG_URL;// str 脚本地址
    _option[4].dwOption = INTERNET_PER_CONN_AUTODISCOVERY_FLAGS;
    _option[5].dwOption = INTERNET_PER_CONN_AUTOCONFIG_SECONDARY_URL;
    _option[6].dwOption = INTERNET_PER_CONN_AUTOCONFIG_RELOAD_DELAY_MINS;
    _option[7].dwOption = INTERNET_PER_CONN_AUTOCONFIG_LAST_DETECT_TIME;
    _option[8].dwOption = INTERNET_PER_CONN_AUTOCONFIG_LAST_DETECT_URL;
    _option[9].dwOption = INTERNET_PER_CONN_FLAGS_UI;

    INTERNET_PER_CONN_OPTION_LISTA _list;
    DWORD dwBufSize = sizeof(INTERNET_PER_CONN_OPTION_LISTA);

    _list.dwSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
    _list.pszConnection = NULL;
    _list.dwOptionCount = 10;
    _list.dwOptionError = 0;
    _list.pOptions = _option;

    bool ret;
    if (ret = InternetQueryOptionA(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &_list, &dwBufSize); ret) {
        auto get_str = [](LPSTR str, std::string& out) {out = str ? std::string(str) : std::string(); if (str) { GlobalFree(str); }   };
        get_str(_option[1].Value.pszValue, m_proxy_server);
        get_str(_option[2].Value.pszValue, m_proxy_bypass);
        get_str(_option[3].Value.pszValue, m_autoconfig_url);
    }

    m_direct = _option[9].Value.dwValue & PROXY_TYPE_DIRECT ? true : false;
    m_proxy = _option[9].Value.dwValue & PROXY_TYPE_PROXY ? true : false;
    m_auto_proxy_url = _option[9].Value.dwValue & PROXY_TYPE_AUTO_PROXY_URL ? true : false;
    m_auto_delect = _option[9].Value.dwValue & PROXY_TYPE_AUTO_DETECT ? true : false;

    return ret;
}

bool WinInternetProxy::set_proxy_option()
{
    INTERNET_PER_CONN_OPTIONA _option[4];
    _option[0].dwOption = INTERNET_PER_CONN_PROXY_SERVER;// str 代理地址和端口
    _option[1].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;// str 代理过滤地址
    _option[2].dwOption = INTERNET_PER_CONN_AUTOCONFIG_URL;// str 脚本地址
    _option[3].dwOption = INTERNET_PER_CONN_FLAGS_UI;// win7 later value 使用代理服务器 代理UI界面状态组合

    _option[0].Value.pszValue = m_proxy_server.data();
    _option[1].Value.pszValue = m_proxy_bypass.data();
    _option[2].Value.pszValue = m_autoconfig_url.data();

    _option[3].Value.dwValue = PROXY_TYPE_DIRECT;
    if (m_proxy) {
        _option[3].Value.dwValue |= PROXY_TYPE_PROXY;
    }
    if (m_auto_proxy_url) {
        _option[3].Value.dwValue |= PROXY_TYPE_AUTO_PROXY_URL;
    }
    if (m_auto_delect) {
        _option[3].Value.dwValue |= PROXY_TYPE_AUTO_DETECT;
    }

    INTERNET_PER_CONN_OPTION_LISTA _list;
    DWORD dwBufSize = sizeof(INTERNET_PER_CONN_OPTION_LISTA);

    _list.dwSize = sizeof(INTERNET_PER_CONN_OPTION_LIST);
    _list.pszConnection = NULL;
    _list.dwOptionCount = 4;
    _list.dwOptionError = 0;
    _list.pOptions = _option;

    if (!InternetSetOptionA(NULL, INTERNET_OPTION_PER_CONNECTION_OPTION, &_list, dwBufSize)) {
        if (GetLastError() == ERROR_INVALID_PARAMETER) {
            fprintf_s(stderr, "%s\r\n", "ERROR_INVALID_PARAMETER");
        }
        return false;
    }

    InternetSetOptionA(nullptr, INTERNET_OPTION_PROXY_SETTINGS_CHANGED, NULL, NULL);
    InternetSetOptionA(nullptr, INTERNET_OPTION_REFRESH, NULL, NULL);
    return true;
}

// win10 proxy ui mapping
void WinInternetProxy::print_current_config()
{
    // ui str
    fprintf_s(stderr, "proxy_server:%s\r\n", m_proxy_server.c_str());
    fprintf_s(stderr, "proxy_bypass:%s\r\n", m_proxy_bypass.c_str());
    fprintf_s(stderr, "autoconfig_url:%s\r\n", m_autoconfig_url.c_str());

    // ui flag
    fprintf_s(stderr, "direct:%s\r\n", std::to_string(m_direct).c_str());
    fprintf_s(stderr, "proxy:%s\r\n", std::to_string(m_proxy).c_str());
    fprintf_s(stderr, "auto_proxy_url:%s\r\n", std::to_string(m_auto_proxy_url).c_str());
    fprintf_s(stderr, "auto_delect:%s\r\n", std::to_string(m_auto_delect).c_str());
}
