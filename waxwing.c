#include "waxwing.h"
#include "stdio.h"

typedef long    WWNDSTYLE;
typedef void*    WWIDGETPROC;
typedef void(*EVENT_WINDOW_CREATE)(HWND);
typedef int(*EVENT_WINDOW_CLOSE)();
typedef void(*EVENT_WINDOW_LBTNDOWN)(POINT);
typedef void(*EVENT_WINDOW_RBTNDOWN)(POINT);
typedef void(*EVENT_BUTTON_CLICKED)();
typedef struct _WIDGETSTRUCT{
    HWND hWnd;
    BYTE type;
}WIDGETSTRUCT;
enum WWidgetType
{
    Widget_Button,
    Widget_Window,
    Widget_Edit,
    Widget_Label,
    Widget_CheckBox,
    Widget_List,
    Widget_Menu
};

typedef struct _EVENTITEM{
    void* fun;
    HWND wnd;
    UINT msg;

    struct _EVENTITEM *next;
}EVENTTABLE;

WCHAR *szWindowClass=L"WWidget";
HINSTANCE hInstance;
EVENTTABLE *eventTable;
WNDPROC OldMsgProc;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    struct _EVENTITEM *item;
    LPVOID *eventFun=NULL;


    if(message==WM_COMMAND||message==WM_NOTIFY)
    {
        for(item=eventTable->next;item;item=item->next)
        {
            //printf("%x\t%x\t%x\t%x\n",lParam,HIWORD(wParam),item->wnd,item->msg);
            if(lParam==item->wnd  && HIWORD(wParam)==item->msg)
            {
                eventFun=item->fun;
                break;
            }
        }

    }else{
        for(item=eventTable->next;item;item=item->next)
        {
            //(((int) *Fun(int,int,int))fun)(1,2,3);
            if(item->msg==message && hWnd==item->wnd)
            {
                eventFun=item->fun;
                break;
            }
        }
    }



    //printf("msg...%d,%d\n",hWnd,message);
    POINT pt;
    //CallWindowProcW(OldMsgProc,hWnd,message,wParam,lParam);
    //printf("wm_command %x,%d,%d,%x\n",hWnd,HIWORD(wParam),LOWORD(wParam),lParam);

    if(eventFun)
    {
        switch(message)
        {
        case WM_COMMAND:

            switch(HIWORD(wParam))
            {
            case BN_CLICKED:
                ((EVENT_BUTTON_CLICKED) eventFun)();
            default:
                //CallWindowProcW(OldMsgProc,hWnd,message,wParam,lParam);
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
            break;
        case WM_CREATE:
            ((EVENT_WINDOW_CREATE) eventFun)(hWnd);
            break;
        case WM_CLOSE:
            if(!((EVENT_WINDOW_CLOSE) eventFun)())ExitProcess(1);
            break;
        case WM_LBUTTONDOWN:
            pt.x=LOWORD(lParam);
            pt.y=HIWORD(lParam);
            ((EVENT_WINDOW_LBTNDOWN)eventFun)(pt);
            break;
        case BN_CLICKED:
            ((EVENT_BUTTON_CLICKED)eventFun)();
            break;
        default:
            break;
            //return DefWindowProc(hWnd, message, wParam, lParam);
        }
        return 0;
    }
    else
        return DefWindowProc(hWnd, message, wParam, lParam);



}

ATOM registerClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style			= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc	= WndProc;
    wcex.cbClsExtra		= 0;
    wcex.cbWndExtra		= 0;
    wcex.hInstance		= hInstance;
    wcex.hIcon			= NULL;//LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT1));
    wcex.hCursor		= LoadCursorW(NULL, IDC_ARROW);
    wcex.hbrBackground	= (HBRUSH)(COLOR_BTNFACE+1);
    wcex.lpszMenuName	= NULL;//MAKEINTRESOURCE(IDC_WIN32PROJECT1);
    wcex.lpszClassName	= szWindowClass;
    wcex.hIconSm		= NULL;//LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    ATOM ret= RegisterClassExW(&wcex);
    return ret;
}
WHWIDGET WcreateWindow(WCHAR* szTitle,
                       DWORD style,
                       DWORD btnMode,
                       WHWIDGET parent,
                       int width,
                       int height)
{
    HWND hWnd;
    //hWnd=parent?((WIDGETSTRUCT*)parent)->hWnd:0;
    hWnd= CreateWindowExW(0,
                          szWindowClass,
                          szTitle,
                          WS_OVERLAPPEDWINDOW&~WS_MAXIMIZEBOX,
                          CW_USEDEFAULT,
                          0,
                          width,
                          height,
                          //parent?((WIDGETSTRUCT*)parent)->hWnd:0,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    if(!hWnd) return 0;

    WIDGETSTRUCT* hWidget=malloc(sizeof(WIDGETSTRUCT));
    hWidget->hWnd=hWnd;
    hWidget->type=Widget_Window;
    return hWidget;
}

WHWIDGET WcreateButton(WCHAR* text,WHWIDGET parent,int x,int y)
{
    HWND hWnd;
    //hWnd=parent?((WIDGETSTRUCT*)parent)->hWnd:0;
    hWnd= CreateWindowExW(0,
                          L"BUTTON",
                          text,
                          WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                          x,
                          y,
                          70,
                          20,
                          parent?((WIDGETSTRUCT*)parent)->hWnd:0,
                          (HMENU)1,
                          hInstance,
                          NULL);
    if(!hWnd) return 0;
    WIDGETSTRUCT* hWidget=malloc(sizeof(WIDGETSTRUCT));
    hWidget->hWnd=hWnd;
    hWidget->type=Widget_Button;
    //OldMsgProc = (WNDPROC)SetWindowLongW (hWnd, GWL_WNDPROC, (LONG)WndProc);
    //ShowWindow(hWnd,SW_SHOW);
    return hWidget;
}
WHWIDGET WcreateEditBox(WHWIDGET parent,WCHAR* text,DWORD style,int x,int y)
{
    HWND hWnd;
    //hWnd=parent?((WIDGETSTRUCT*)parent)->hWnd:0;
    hWnd= CreateWindowExW(0,
                          L"EDIT",
                          text,
                          (WS_VISIBLE | WS_CHILD|ES_AUTOHSCROLL|WS_TABSTOP|WS_BORDER)|style ,
                          x,
                          y,
                          70,
                          20,
                          parent?((WIDGETSTRUCT*)parent)->hWnd:0,
                          (HMENU)1,
                          hInstance,
                          NULL);
    if(!hWnd) return 0;
    WIDGETSTRUCT* hWidget=malloc(sizeof(WIDGETSTRUCT));
    hWidget->hWnd=hWnd;
    hWidget->type=Widget_Edit;
    //OldMsgProc = (WNDPROC)SetWindowLongW (hWnd, GWL_WNDPROC, (LONG)WndProc);
    //ShowWindow(hWnd,SW_SHOW);
    return hWidget;
}
BOOL WshowWidget(WHWIDGET widget,DWORD showMode)
{
    return ShowWindow(((WIDGETSTRUCT* )widget)->hWnd,showMode);
}
int WgetWidgetText(WHWIDGET widget,LPCWSTR buf,size_t size)
{
    return GetWindowTextW(((WIDGETSTRUCT* )widget)->hWnd,buf,size);
}
HWND WgetWidgetHWND(WHWIDGET widget)
{
    return ((WIDGETSTRUCT* )widget)->hWnd;
}

int WattachEvent(WHWIDGET widget,
                 DWORD eventType,
                 LPVOID fun)
{
    if(!fun) return 1;
    UINT msg=0;
    switch(eventType)
    {
    case WEvent_Wnd_Clicked:
        msg=WM_LBUTTONDOWN;
        break;
    case WEvent_Wnd_Close:
        msg=WM_CLOSE;
        break;
    case WEvent_Wnd_LBtnDown:
        msg=WM_LBUTTONDOWN;
        break;
    case WEvent_Wnd_Load:
        msg=WM_CREATE;
        break;
    case WEvent_Button_Clicked:
        msg=BN_CLICKED;
        break;
    default:
        return 2;
    }
    //if(!msg)return 2;
    struct _EVENTITEM *item;
    item=malloc(sizeof(struct _EVENTITEM));
    printf("%d\n",item);
    item->fun=fun;

    item->msg=msg;
    //printf("hwnd=%d\n",widget);
    item->wnd=((WIDGETSTRUCT*)widget)->hWnd;

    item->next=eventTable->next;

    eventTable->next=item;

    return 0;
}

int WmessageLoop()
{
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

int WinitWaxwing()
{
    ATOM ret;
    hInstance=GetModuleHandleW(NULL);
    ret=registerClass(hInstance);
    if(!ret) return 1;
    eventTable=malloc(sizeof(struct _EVENTITEM));
    eventTable->next=NULL;
    return 0;
}

int WdeleteWaxwing()
{

}
