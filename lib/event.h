#ifndef _EVENT_H_
#define _EVENT_H_
#include "object.h"
#include "strings.h"

class ClickEvent
{
public:
    virtual void click(Object & sender) = 0;
    virtual void dblClick(Object & sender) = 0;    
};

class PaintEvent
{
public:
    virtual void paint(Object & sender, HDC &hDC) = 0;
};

class MouseEvent
{
public:
    virtual void mouseDown(Object & sender, int x, int y, int button) = 0;
    virtual void mouseUp(Object & sender, int x, int y, int button) = 0;
    virtual void mouseMove(Object & sender, int x, int y) = 0;
};

class KeyEvent
{
public:
    virtual void keyDown(Object & sender, int key, int state) = 0;
    virtual void keyUp(Object & sender, int key, int state) = 0;
    virtual void keyPress(Object & sender, char key, int state) = 0;
};

class ResizeEvent
{
public:
    virtual void resize(Object & sender, int width, int height) = 0;
};

class DragDropEvent
{
public:
    virtual void drop(const String& urls) = 0;
};

class ChangeEvent
{
public:
    virtual void change(Object & sender) = 0;    
};
    
#endif
