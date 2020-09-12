#include "window.h"

static std::list<Window*> windowList;

Window::Window()
{
    windowList.push_back(this);
    done = false;
    resizable = true;
    menu = NULL;
}

Window::~Window()
{
    windowList.remove(this);
}

void Window::terminate(int result)
{
    PostQuitMessage(result);
}

void Window::close()
{
    if(windowList.size() > 1)
    {
        hide();
        done = true;
    }
    else
        terminate(0);
}

void Window::setMenu(MainMenu * menu)
{
    this->menu = menu;
    SetMenu(getHandle(), menu->getHandle());
}

void Window::setIcon(String resource)
{
    HICON icon;

    icon = LoadIcon(GetModuleHandle(NULL), resource.c_str());
    SendMessage(getHandle(), WM_SETICON, 0, (WPARAM)icon);
    DeleteObject(icon);
}

void Window::setResizable(bool enable)
{
    DWORD style;
    resizable = enable;
    if(getHandle() == 0)
        return;
    style = GetWindowLong(getHandle(), GWL_STYLE);
    if(enable)
        style |= WS_SIZEBOX;
    else
        style &= ~WS_SIZEBOX;
    SetWindowLong(getHandle(), GWL_STYLE, style);
}

void Window::setButtonEnabled(int button, bool enable)
{
    DWORD style, oldStyle;

    if(getHandle() != 0)
    {
        style = GetWindowLong(getHandle(), GWL_STYLE);
        oldStyle = style;
        if((button & BUTTON_MINIMIZE) > 0)
        {
            if(enable)
                style |= WS_MINIMIZEBOX;
            else
                style &= ~WS_MINIMIZEBOX;
        }
        if((button & BUTTON_MAXIMIZE) > 0)
        {
            if(enable)
                style |= WS_MAXIMIZEBOX;
            else
                style &= ~WS_MAXIMIZEBOX;
        }
        if((button & BUTTON_CLOSE) > 0)
        {
            if(enable)
                EnableMenuItem(GetSystemMenu(getHandle(), FALSE), SC_CLOSE, MF_ENABLED);
            else
                EnableMenuItem(GetSystemMenu(getHandle(), FALSE), SC_CLOSE, MF_DISABLED);
        }
        if(oldStyle != style)
            SetWindowLong(getHandle(), GWL_STYLE, style);
    }
}

DWORD Window::getStyle()
{
    DWORD style;

    style = WS_SIZEBOX | WS_DLGFRAME | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
    if(!resizable)
        style &= ~WS_SIZEBOX;
    return style;
}

bool Window::create(String className, int width, int height,
                    String title, Control * parent)
{
    if(!registerClass(className))
        return false;
    if(!createControl(0, getStyle(),
                      className, title,
                      (GetSystemMetrics(SM_CXSCREEN) - width ) / 2,
                      (GetSystemMetrics(SM_CYSCREEN) - height) / 2,
                      width, height, parent))
        return false;
    done = false;
    return true;
}

LRESULT Window::message(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_COMMAND:
        if(menu != NULL)
            menu->menuProc(getHandle(), uMsg, wParam, lParam);
        break;
    case WM_CLOSE:
    {
        bool canClose = true;
        onClose(canClose);
        if(canClose)
            close();
        return 0;
    }
    case WM_MOVE:
        move((short)LOWORD(lParam), (short)HIWORD(lParam));
        return 0;
    }
    return Control::message(uMsg, wParam, lParam);
}

int Window::messageBox(String text, String title, UINT type)
{
    return MessageBox(getHandle(), text.c_str(), title.c_str(), type);
}

void Window::showMessage(String text)
{
    messageBox(text.c_str(), getText(), 0);
}

int Window::run()
{
    MSG msg;

    while(!done && GetMessage(&msg, NULL, 0, 0))
    {
        if(!IsDialogMessage(getHandle(), &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    if(done)
    {
        done = false;
        return 0;
    }
    return msg.wParam;
}

