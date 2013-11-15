#include "StdAfx.h"
#include "CefJamClientHandler.h"


#if defined(DEBUG)||defined(_DEBUG)

#pragma comment(lib,"../../cef/lib/libcef.lib")
#pragma comment(lib,"../../cef/lib/libcef_dll_wrapper.lib")

#else

#pragma comment(lib,"../../cef/librel/libcef.lib")
#pragma comment(lib,"../../cef/librel/libcef_dll_wrapper.lib")

#endif

CefJamClientHandler::CefJamClientHandler(void)
{

}


CefJamClientHandler::~CefJamClientHandler(void)
{

}

void CefJamClientHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) 
{
	REQUIRE_UI_THREAD();
	m_Browser=browser;
}
