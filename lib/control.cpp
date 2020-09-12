#include "control.h"

static int getKeyState()
{
    SHORT temp;
    int state = 0;

    temp = GetKeyState(VK_LCONTROL);
    if(temp < 0)
        state |= LeftCtrl;
    temp = GetKeyState(VK_RCONTROL);
    if(temp < 0)
        state |= RightCtrl;
    temp = GetKeyState(VK_LSHIFT);
    if(temp < 0)
        state |= LeftShift;
    temp = GetKeyState(VK_RSHIFT);
    if(temp < 0)
        state |= RightShift;
    return state;
}

Control::Control()
{
    className = "";
    enabled = true;
    wndProc = DefWindowProc;
    font = new Font(this);
    popupMenu = NULL;
}

Control::~Control()
{
    delete font;
    if(hWnd != 0)
        DestroyWindow(hWnd);
}

LRESULT STDCALL Control::winProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Control * control = NULL;

    control = (Control*)GetWindowLongPtr(hWnd, GWL_USERDATA);
    if(uMsg == WM_NCCREATE)
    {
        LPCREATESTRUCT lpcs;

        lpcs = (LPCREATESTRUCT)lParam;
        control = (Control*)lpcs->lpCreateParams;
        SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG)control);
        control->hWnd = hWnd;
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    if(control != NULL)
        return control->message(uMsg, wParam, lParam);
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void Control::setPopupMenu(PopupMenu * popupMenu)
{
    this->popupMenu = popupMenu;
}

HWND Control::getHandle()
{
    return hWnd;
}

Control * Control::getParent()
{
    return parent;
}

String Control::getClassName()
{
    return className;
}

void Control::setFont(Font * font)
{
    if(font == NULL)
        return;
    delete this->font;
    this->font = font;
    if(getHandle() != 0)
        SendMessage(getHandle(), WM_SETFONT, (WPARAM)font->getHandle(), 0);
}

Font& Control::getFont()
{
    return *font;
}

void Control::addStyle(DWORD style)
{
    DWORD currStyle;

    currStyle = (DWORD)GetWindowLong(getHandle(), GWL_STYLE);
    currStyle |= style;
    SetWindowLong(getHandle(), GWL_STYLE, currStyle);
}

void Control::delStyle(DWORD style)
{
    DWORD currStyle;

    currStyle = (DWORD)GetWindowLong(getHandle(), GWL_STYLE);
    currStyle &= ~style;
    SetWindowLong(getHandle(), GWL_STYLE, currStyle);
}

void Control::addExStyle(DWORD exStyle)
{
    DWORD currExStyle;

    currExStyle = (DWORD)GetWindowLong(getHandle(), GWL_EXSTYLE);
    currExStyle |= exStyle;
    SetWindowLong(getHandle(), GWL_EXSTYLE, currExStyle);
}

void Control::delExStyle(DWORD exStyle)
{
    DWORD currExStyle;

    currExStyle = (DWORD)GetWindowLong(getHandle(), GWL_EXSTYLE);
    currExStyle &= ~exStyle;
    SetWindowLong(getHandle(), GWL_EXSTYLE, currExStyle);
}

bool Control::registerClass(String className)
{
    WNDCLASS wc;

    if(GetClassInfo(GetModuleHandle(NULL), className.c_str(), &wc) != 0)
    {
        this->className = className;
        return true; //class already registered
    }
    wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = winProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hIcon  = LoadIcon(GetModuleHandle(NULL), "MAINICON");
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = className.c_str();
    this->className = className;
    return RegisterClass(&wc);
}

bool Control::createControl(DWORD exStyle, DWORD style, String className,
                            String text, int x, int y,
                            int width, int height,  Control * parent)
{
    HWND hWnd_parent = 0;

    this->parent = parent;
    if(parent != NULL)
    {
        hWnd_parent = parent->getHandle();
        style |= WS_CHILD;
        if(this->className == "")
            style |= WS_VISIBLE;
    }

    hWnd = CreateWindowEx(exStyle, className.c_str(), text.c_str(), style, x, y, width, height,
                          hWnd_parent, NULL, GetModuleHandle(NULL), this);

    SendMessage(getHandle(), WM_SETFONT, (WPARAM)font->getHandle(), 0);

    if(this->className == "")
    {
        SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG)this);
        this->className = className;
        wndProc = (WNDPROC)SetWindowLongPtr(hWnd, GWL_WNDPROC, (LONG)winProc);
        resize(width, height);
    }
    return getHandle() != 0;
}

void Control::setText(String text)
{
    SetWindowText(hWnd, text.c_str());
}

String Control::getText()
{
    String result;
    int len = GetWindowTextLength(hWnd);
    char * _text = new char[len + 1];
    GetWindowText(hWnd, _text, len + 1);
    result = _text;
    delete _text;
    return result;
}

int Control::getClientWidth()
{
    RECT rect;

    GetClientRect(hWnd, &rect);
    return rect.right - rect.left;
}

Control* Control::getControlAt(int x, int y)
{
    HWND hWndControl;
    POINT pt;

    pt.x = x;
    pt.y = y;
    hWndControl = WindowFromPoint(pt);
    return (Control*)GetWindowLong(hWndControl, GWL_USERDATA);
}

Control* Control::getRelativeControlAt(int x, int y, Control* parent)
{
    HWND hWndControl, hWndParent = 0;
    POINT pt;

    pt.x = x;
    pt.y = y;
    if(parent != NULL)
        hWndParent = parent->getHandle();
    hWndControl = ChildWindowFromPoint(hWndParent, pt);
    return (Control*)GetWindowLong(hWndControl, GWL_USERDATA);
}

int Control::getClientHeight()
{
    RECT rect;

    GetClientRect(hWnd, &rect);
    return rect.bottom - rect.top;
}

int Control::getWidth()
{
    RECT rect;

    GetWindowRect(hWnd, &rect);
    return rect.right - rect.left;
}

int Control::getHeight()
{
    RECT rect;

    GetWindowRect(hWnd, &rect);
    return rect.bottom - rect.top;
}

void Control::getClientRect(RECT & rect)
{
    GetClientRect(hWnd, &rect);
}

int Control::getX()
{
    RECT rect;

    GetWindowRect(hWnd, &rect);
    if(parent != NULL)
        ScreenToClient(parent->getHandle(), (LPPOINT)(&rect));
    return rect.left;
}

int Control::getY()
{
    RECT rect;

    GetWindowRect(hWnd, &rect);
    if(parent != NULL)
        ScreenToClient(parent->getHandle(), (LPPOINT)(&rect));
    return rect.top;
}

void Control::setPosition(int x, int y)
{
    SetWindowPos(getHandle(), 0, x, y, 0, 0, SWP_NOSIZE);
}

void Control::setSize(int width, int height)
{
    SetWindowPos(getHandle(), 0, 0, 0, width, height, SWP_NOMOVE);
}

void Control::setFocus()
{
    SetFocus(getHandle());
}

void Control::setDragDrop(bool enabled)
{
    if(enabled)
        DragAcceptFiles(getHandle(), TRUE);
    else
        DragAcceptFiles(getHandle(), FALSE);
}

void Control::show()
{
    ShowWindow(hWnd, SW_SHOW);
}

void Control::hide()
{
    ShowWindow(hWnd, SW_HIDE);
}

void Control::invalidate()
{
    InvalidateRect(hWnd, NULL, FALSE);
}

bool Control::isEnabled()
{
    return enabled;
}

void Control::setEnabled(bool enabled)
{
    this->enabled = enabled;
    EnableWindow(hWnd, (enabled) ? TRUE : FALSE);
}


void Control::addPaintEvent(PaintEvent * paint)
{
    paintEvent.push_back(paint);
}

void Control::addMouseEvent(MouseEvent * mouse)
{
    mouseEvent.push_back(mouse);
}

void Control::addKeyEvent(KeyEvent * key)
{
    keyEvent.push_back(key);
}

void Control::addResizeEvent(ResizeEvent * resize)
{
    resizeEvent.push_back(resize);
}

void Control::addDragDropEvent(DragDropEvent * dragdrop)
{
    dropEvent.push_back(dragdrop);
}

void Control::delPaintEvent(PaintEvent * paint)
{
    paintEvent.remove(paint);
}

void Control::delMouseEvent(MouseEvent * mouse)
{
    mouseEvent.remove(mouse);
}

void Control::delKeyEvent(KeyEvent * key)
{
    keyEvent.remove(key);
}

void Control::delResizeEvent(ResizeEvent * resize)
{
    resizeEvent.remove(resize);
}

void Control::delDragDropEvent(DragDropEvent * dragdrop)
{
    dropEvent.remove(dragdrop);
}

LRESULT Control::message(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        // Last message to a window so we de-subclass ourselves.
    case WM_NCDESTROY:
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)wndProc);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, 0);
        break;
        // Sent by IsDialogMessage() API to determine what keys the control wants.
        // We use this to forward the tab key so it selects the next control.
    case WM_GETDLGCODE:
    {

        LRESULT lrResult = 0;
        lrResult = CallWindowProc(wndProc, hWnd, uMsg, wParam, lParam);
        lrResult &= ~(DLGC_HASSETSEL | DLGC_WANTTAB);
        if(lParam && ((LPMSG)lParam)->message == WM_KEYDOWN &&
                ((LPMSG)lParam)->wParam == VK_TAB)
            lrResult &= ~DLGC_WANTMESSAGE;
        return lrResult;
    }
    case WM_PAINT:
    {
        /*HDC hDC;
        PAINTSTRUCT ps;

        hDC = BeginPaint(getHandle(), &ps);
        paint(hDC);
        EndPaint(getHandle(), &ps);*/
        return CallWindowProc(wndProc, hWnd, uMsg, wParam, lParam);
    }
    case WM_COMMAND:
    {
        Control * control;
        control = (Control*)GetWindowLongPtr((HWND)lParam, GWL_USERDATA);
        if(popupMenu != NULL)
            popupMenu->menuProc(hWnd, uMsg, wParam, lParam);
        if(control != NULL)
            control->command(wParam, lParam);
        break;
    }
    case WM_MOUSEMOVE:
    {
        int y, x;
        MouseEvent * mouse;
        std::list<MouseEvent*>::iterator ite;

        x = (short)LOWORD(lParam);
        y = (short)HIWORD(lParam);
        mouseMove(x, y);

        for(ite = mouseEvent.begin(); ite != mouseEvent.end(); ite++)
        {
            mouse = *ite;
            mouse->mouseMove(*this, x, y);
        }
        break;
    }
    case WM_DROPFILES:
    {
        int fileCount, i;
        UINT bufferSize;
        char * buffer;
        DragDropEvent * dragdrop;
        std::list<DragDropEvent*>::iterator ite;
        String urls;
        HDROP hDrop;

        hDrop = (HDROP)wParam;
        fileCount = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
        for(i = 0; i < fileCount; i++)
        {
            bufferSize = DragQueryFile(hDrop, i, NULL, 0);
            buffer = new char[bufferSize + 1];
            DragQueryFile(hDrop, i, buffer, bufferSize + 1);
            if(i > 0)
            {
                urls += "\n";
                urls += buffer;
            }
            else
                urls = buffer;
            delete[] buffer;
        }
        drop(urls);
        for(ite = dropEvent.begin(); ite != dropEvent.end(); ite++)
        {
            dragdrop = *ite;
            dragdrop->drop(urls);
        }
        break;
    }
    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
    {
        int button = 1;
        int y, x;
        MouseEvent * mouse;
        std::list<MouseEvent*>::iterator ite;
        POINT pt;
        HMENU hmenuTrackPopup;

        x = (short)LOWORD(lParam);
        y = (short)HIWORD(lParam);
        if(uMsg == WM_MBUTTONDOWN)
            button = 2;
        else if(uMsg == WM_RBUTTONDOWN)
            button = 3;
        mouseDown(x, y, button);
        for(ite = mouseEvent.begin(); ite != mouseEvent.end(); ite++)
        {
            mouse = *ite;
            mouse->mouseDown(*this, x, y, button);
        }
        if(uMsg == WM_RBUTTONDOWN && popupMenu != NULL)
        {
            pt.x = x;
            pt.y = y;
            hmenuTrackPopup = popupMenu->getHandle();
            ClientToScreen(getHandle(), &pt);
            TrackPopupMenu(hmenuTrackPopup,
                           TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL, pt.x, pt.y,
                           0, getHandle(), NULL);
        }
        break;
    }
    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP:
    {
        int button = 1;
        int y, x;
        MouseEvent * mouse;
        std::list<MouseEvent*>::iterator ite;

        x = (short)LOWORD(lParam);
        y = (short)HIWORD(lParam);
        if(uMsg == WM_MBUTTONDOWN)
            button = 2;
        else if(uMsg == WM_RBUTTONDOWN)
            button = 3;
        mouseUp(x, y, button);
        for(ite = mouseEvent.begin(); ite != mouseEvent.end(); ite++)
        {
            mouse = *ite;
            mouse->mouseUp(*this, x, y, button);
        }
        break;
    }
    case WM_KEYDOWN:
    {
        KeyEvent * key;
        int state;
        std::list<KeyEvent*>::iterator ite;

        state = getKeyState();
        keyDown(wParam, state);
        for(ite = keyEvent.begin(); ite != keyEvent.end(); ite++)
        {
            key = *ite;
            key->keyDown(*this, wParam, state);
        }
        break;
    }
    case WM_KEYUP:
    {
        KeyEvent * key;
        int state;
        std::list<KeyEvent*>::iterator ite;

        state = getKeyState();
        keyUp(wParam, state);
        for(ite = keyEvent.begin(); ite != keyEvent.end(); ite++)
        {
            key = *ite;
            key->keyUp(*this, wParam, state);
        }
        break;
    }
    case WM_CHAR:
    {
        KeyEvent * key;
        int state;
        std::list<KeyEvent*>::iterator ite;

        state = getKeyState();
        keyPress((char)wParam, state);
        for(ite = keyEvent.begin(); ite != keyEvent.end(); ite++)
        {
            key = *ite;
            key->keyPress(*this, (char)wParam, state);
        }
        break;
    }

    case WM_SIZE:
    {
        int width, height;
        ResizeEvent * _resize;
        std::list<ResizeEvent*>::iterator ite;

        width = (short)LOWORD(lParam);
        height = (short)HIWORD(lParam);
        resize(width, height);
        for(ite = resizeEvent.begin(); ite != resizeEvent.end(); ite++)
        {
            _resize = *ite;
            _resize->resize(*this, width, height);
        }
        break;
    }
    }
    return CallWindowProc(wndProc, hWnd, uMsg, wParam, lParam);
}

