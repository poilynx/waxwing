#include <stdio.h>
#include <windows.h>
#include "waxwing.h"
WHWIDGET hForm;
WHWIDGET hButton1,hEdit;
void fun1(POINT pos)
{
    printf("left btn down x=%d,y=%d\n",pos.x,pos.y);
    return;
}
int fun2()
{
    puts("close event");
    return 0;

}
void fun3()
{
    wchar_t buff[100];
    WgetWidgetText(hEdit,buff,100);
    MessageBoxW(WgetWidgetHWND(hForm),buff,L"text",MB_OK);
    return ;
}


int main(void)
{
    int ret;

    ret=WinitWaxwing();
    if(ret)
    {
        puts("init error");
        return 1;
    }
    //printf("ret=%x\n",ret);
    hForm=WcreateWindow(L"Message",NULL,NULL,NULL,400,300);
    if(!hForm)
    {
        puts("createwindow error");
        return 1;
    }
    //hButton=WcreateButton(L"click me",hFrom,10,10);

    hButton1=WcreateButton(L"Close",hForm,160,240);
    if(!hButton1)
    {
        puts("createbutton error");
        return 1;
    }
    hEdit=WcreateEditBox(hForm,L"",ES_MULTILINE|ES_READONLY,10,10);
    if(!hEdit)
    {
        puts("createedit error");
        return 1;
    }
    MoveWindow(WgetWidgetHWND(hEdit),8,8,376,220,TRUE);
    WattachEvent(hButton1,WEvent_Button_Clicked,fun3);
    //WattachEvent(hEdit,WEvent_Button_Clicked,fun4);
    //WattachEvent(hForm,WEvent_Wnd_LBtnDown,fun1);
    //WattachEvent(hForm,WEvent_Wnd_Close,fun2);
    //printf("hForm=%x\n",hForm);
    WshowWidget(hForm,SW_SHOW);
    WmessageLoop();
    return 0;
}
