#ifndef __CEFJAM_CLIENT_HANDLER_H__
#define __CEFJAM_CLIENT_HANDLER_H__

#include "../../cef/include/cef_base.h"
#include "../../cef/include/cef_app.h"
#include "../../cef/include/cef_client.h"
#include "../../cef/include/cef_life_span_handler.h"

class CefJamClientHandler:	public CefClient,
							public CefLifeSpanHandler
{
public:
	CefJamClientHandler(void);
	~CefJamClientHandler(void);

protected :
	CefRefPtr<CefBrowser> m_Browser ;

public:
	CefRefPtr<CefBrowser> getBrowser ( )  {  return m_Browser ;  }

	//CefClient methods
	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() {
		return this;
	}
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) {
		m_Browser=browser;
	}

	//Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(CefJamClientHandler);
	//Include the default locking implementation.
	IMPLEMENT_LOCKING(CefJamClientHandler);
};

#endif//__CEFJAM_CLIENT_HANDLER_H__



