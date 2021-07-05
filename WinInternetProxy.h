#pragma once

#include <string>

class WinInternetProxy
{
public:
    std::string query_http_version();

    bool query_proxy_option();

    bool set_proxy_option();

    void print_current_config();

    // ui str
    std::string m_proxy_server;
    std::string m_proxy_bypass;
    std::string m_autoconfig_url;

    // ui flag
    bool m_direct = false;// no set
    bool m_proxy = false;
    bool m_auto_proxy_url = false;
    bool m_auto_delect = false;
};