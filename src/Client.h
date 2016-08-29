#ifndef CLIENT_H_
#define CLIENT_H_

#include "include/cef_app.h"
#include "include/cef_client.h"
#include "include/cef_browser.h"

#include "JobsManager.h"

#include <queue>

namespace cefpdf {

class Client : public CefApp,
               public CefBrowserProcessHandler,
               public CefClient,
               public CefLifeSpanHandler,
               public CefLoadHandler
{
    public:

    Client(bool exitOnDone = false);

    // Run message loop
    void Run();

    // Stop message loop and shutdown
    void Stop();

    // Queue new job to process
    void QueueJob(CefRefPtr<Job> job);

    // CefApp methods:
    virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override;
    virtual void OnRegisterCustomSchemes(CefRefPtr<CefSchemeRegistrar> registrar) override;

    // CefBrowserProcessHandler methods:
    virtual CefRefPtr<CefPrintHandler> GetPrintHandler() override;
    virtual void OnContextInitialized() override;
    virtual void OnBeforeChildProcessLaunch(CefRefPtr<CefCommandLine> command_line) override;

    // CefClient methods:
    virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler();
    virtual CefRefPtr<CefLoadHandler> GetLoadHandler();
    virtual CefRefPtr<CefRenderHandler> GetRenderHandler();

    // CefLifeSpanHandler methods:
    virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
    virtual bool DoClose(CefRefPtr<CefBrowser> browser) override;
    virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

    // CefLoadHandler methods:
    virtual void OnLoadStart(
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame
    ) override;
    virtual void OnLoadEnd(
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        int httpStatusCode
    ) override;
    virtual void OnLoadError(
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        ErrorCode errorCode,
        const CefString& errorText,
        const CefString& failedUrl
    ) override;

    private:

    CefRefPtr<CefPrintHandler> m_printHandler;
    CefRefPtr<CefRenderHandler> m_renderHandler;

    CefSettings m_settings;
    CefWindowInfo m_windowInfo;
    CefRequestContextSettings m_contextSettings;
    CefBrowserSettings m_browserSettings;

    bool m_exitOnDone = false;
    bool m_shouldStop = false;

    bool StartProcess();

    // Jobs waiting to be processed
    std::queue<CefRefPtr<Job>> m_jobsQueue;

    // Jobs currently processed
    CefRefPtr<JobsManager> m_jobsManager;

    int m_processCount = 0;
    int m_browserCount = 0;

    // Include the default reference counting implementation.
    IMPLEMENT_REFCOUNTING(Client);
};

} // namespace cefpdf

#endif // CLIENT_H_