# WinInternetProxy

`WinInternetProxy` 是一款 Windows 系统代理配置功能项，可使用代码设置系统代理功能，可应用在 v2ray、trojan-go 等需要设置 `系统IE代理` 类似的工具中。

### 如何使用？
```c++
#include <iostream>
#include "WinInternetProxy.h"

int main()
{
	// 实例化一个类
	WinInternetProxy proxy;

	// query proxy
	// 查询 windows 系统代理 ui 界面信息
	proxy.query_proxy_option();
	proxy.print_current_config();

	// setting proxy
	// 清除 pac 代理，仅使用系统代理
	proxy.m_proxy_server = "127.0.0.1:1080";
	proxy.m_proxy_bypass = "localhost;127.*;192.168.*";
	proxy.m_autoconfig_url = "";//pac

	proxy.m_proxy = true;
	proxy.m_auto_proxy_url = false;
	proxy.m_auto_delect = false;

	// 设置配置项
	proxy.set_proxy_option();
	proxy.print_current_config();
	return 0;
}
```
