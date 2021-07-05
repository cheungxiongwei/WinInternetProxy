#include <iostream>
#include "WinInternetProxy.h"

int main()
{
	WinInternetProxy proxy;

	// query proxy
	proxy.query_proxy_option();
	proxy.print_current_config();

	// setting proxy
	proxy.m_proxy_server = "127.0.0.1:1080";
	proxy.m_proxy_bypass = "localhost;127.*;192.168.*";
	proxy.m_autoconfig_url = "";//pac

	proxy.m_proxy = true;
	proxy.m_auto_proxy_url = false;
	proxy.m_auto_delect = false;

	proxy.set_proxy_option();
	proxy.print_current_config();
	return 0;
}