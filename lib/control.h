#ifndef _CONTROL_H_
#define _CONTROL_H_
#include <windows.h>
#include <list>
#include "strings.h"
#include "object.h"
#include "event.h"
#include "font.h"
#include "menu.h"

/*
 * representação de teclas de controle 
 * por exemplo quando você pressiona Ctrl+P o state(estado) é
 * o LeftCtrl
**/
typedef enum
{
    LeftCtrl   = 0x01,
    RightCtrl  = 0x02,
    LeftShift  = 0x04,
    RightShift = 0x08,
    LeftAlt    = 0x10,
    RightAlt   = 0x20,
    FnKey      = 0x40,
} KeyState;

class Control: public Object
{
private:
    HWND hWnd;
    String className;
    Control * parent;
    std::list<PaintEvent*> paintEvent;
    std::list<MouseEvent*> mouseEvent;
    std::list<KeyEvent*> keyEvent;
    std::list<ResizeEvent*> resizeEvent;
    std::list<DragDropEvent*> dropEvent;
    bool enabled;
    WNDPROC wndProc;
    Font * font;
    PopupMenu * popupMenu;
    static LRESULT STDCALL winProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
    //internal events
    virtual void paint(HDC &hDC) {}
    virtual void mouseDown(int x, int y, int button) {}
    virtual void mouseUp(int x, int y, int button) {}
    virtual void mouseMove(int x, int y) {}
    virtual void keyDown(int key, int state) {}
    virtual void keyUp(int key, int state) {}
    virtual void keyPress(char key, int state) {}
    virtual void resize(int width, int height) {}
    virtual void drop(const String& urls) {}
    virtual void command(WPARAM wParam, LPARAM lParam) {}
    virtual LRESULT message(UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
    Control();
    ~Control();
    HWND getHandle();
    Control * getParent();
    String getClassName();
    int getClientWidth();
    int getClientHeight();
    int getWidth();
    int getHeight();
    int getX();
    int getY();
    void setPosition(int x, int y);
    void setSize(int width, int height);
    void getClientRect(RECT & rect);
    Control* getControlAt(int x, int y);
    Control* getRelativeControlAt(int x, int y, Control* parent);
    void show();
    void hide();
    void setFont(Font * font);
    void setPopupMenu(PopupMenu * popupMenu);
    Font& getFont();
    void invalidate();
    void setFocus();
    bool isEnabled();
    void setEnabled(bool enabled);
    void setText(String text);
    void setDragDrop(bool enabled);
    String getText();
    void addStyle(DWORD style);
    void delStyle(DWORD style);
    void addExStyle(DWORD exStyle);
    void delExStyle(DWORD exStyle);
    bool registerClass(String className);
    bool createControl(DWORD exStyle, DWORD style, String className,
                       String text, int x, int y, int width, int height,
                       Control * parent);
    void addPaintEvent(PaintEvent * paint);
    void addMouseEvent(MouseEvent * mouse);
    void addKeyEvent(KeyEvent * key);
    void addResizeEvent(ResizeEvent * resize);
    void addDragDropEvent(DragDropEvent * dragdrop);
    void delPaintEvent(PaintEvent * paint);
    void delMouseEvent(MouseEvent * mouse);
    void delKeyEvent(KeyEvent * key);
    void delResizeEvent(ResizeEvent * resize);
    void delDragDropEvent(DragDropEvent * dragdrop);
};

#endif
