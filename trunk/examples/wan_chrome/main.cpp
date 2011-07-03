
#include <tchar.h>
#include <windows.h>
#include <initguid.h>
#include <oleacc.h>

#include <atlbase.h>

#include "../wanui_res/resource.h"

#include "base/at_exit.h"
#include "base/command_line.h"
#include "base/file_path.h"
#include "base/path_service.h"

#include "view/base/resource_bundle.h"
#include "view/focus/accelerator_handler.h"
#include "view/window/window.h"

#include "frame/browser_frame_win.h"
#include "frame/browser_view.h"

CComModule _Module;

// 程序入口.
int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    HRESULT hRes = OleInitialize(NULL);

    // this resolves ATL window thunking problem when Microsoft Layer
    // for Unicode (MSLU) is used.
    ::DefWindowProc(NULL, 0, 0, 0L);

    base::AtExitManager exit_manager;
    base::CommandLine::Init(0, NULL);

    FilePath res_dll;
    PathProvider(base::DIR_EXE, &res_dll);
    res_dll = res_dll.Append(L"wanui_res.dll");
    ResourceBundle::InitSharedInstance(res_dll);

    view::AcceleratorHandler handler;
    MessageLoop loop(MessageLoop::TYPE_UI);
    view::Window* frame = BrowserFrameWin::Create(new BrowserView());
    frame->Show();
    MessageLoopForUI::current()->Run(&handler);

    ResourceBundle::CleanupSharedInstance();
    OleUninitialize();

    return 0;
}


// 提升公共控件样式.
#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif