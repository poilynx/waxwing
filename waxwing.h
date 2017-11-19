#ifndef WAXWING_H
#define WAXWING_H
#include <windows.h>
typedef LPVOID  WHWIDGET;
enum EVENTTYPE{
    WEvent_Wnd_Load=1,
    WEvent_Wnd_Clicked,
    WEvent_Wnd_Close,
    WEvent_Wnd_Moved,
    WEvent_Wnd_Focused,
    WEvent_Wnd_LBtnDown,
    WEvent_Wnd_RBtnDown,
    WEvent_Button_Clicked
};

extern WHWIDGET WcreateWindow(WCHAR* szTitle,
                              DWORD style,
                              DWORD btnMode,
                              WHWIDGET parent,
                              int width,
                              int height);
extern WHWIDGET WcreateButton(WCHAR* text,
                              WHWIDGET parent,
                              int x,
                              int y);

extern WHWIDGET WcreateEditBox(WHWIDGET parent,
                        WCHAR* text,
                        DWORD style,
                        int x,
                        int y);

extern int WattachEvent(WHWIDGET widget,
                        DWORD event,
                        LPVOID eventProc);
extern int WmessageLoop();
extern int WinitWaxwing();
extern int WdeleteWaxwing();
extern BOOL WshowWidget(WHWIDGET widget,DWORD showMode);
extern int WgetWidgetText(WHWIDGET widget,LPCWSTR buf,size_t size);
extern HWND WgetWidgetHWND(WHWIDGET widget);
#endif // WAXWING_H
