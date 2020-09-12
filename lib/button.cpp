#include "button.h"
#include "imageUtil.h"

Button::Button()
{
    hImage = 0;
    showCaption = true;
}


DWORD Button::getStyle()
{
    return BS_PUSHBUTTON | WS_TABSTOP;
}

bool Button::create(int x, int y, int width, int height,
            String text, Control * parent)
{
    if(!createControl(0, getStyle(), "Button", text, x, y, width, height, parent))
        return false;
    if(hImage != 0)
    {
        setImage(hImage);
        if(!showCaption)
            setShowCaption(showCaption);
    }
    return true;
}

void Button::addClickEvent(ClickEvent* click)
{
    clickEvent.push_back(click);
}

void Button::command(WPARAM wParam, LPARAM lParam)
{
    switch(HIWORD(wParam))
    {
    case BN_CLICKED:
    {
        std::list<ClickEvent*>::iterator ite;
        ClickEvent * click;

        for(ite = clickEvent.begin(); ite != clickEvent.end(); ite++)
        {
            click = *ite;
            click->click(*this);
        }
        break;
    }
    }
    Control::command(wParam, lParam);
}

void Button::setShowCaption(bool show)
{
    showCaption = show;
    if(getHandle() == 0)
        return;
    if(!show)
        addStyle(BS_BITMAP);
    else
        delStyle(BS_BITMAP);
}

void Button::setImage(HBITMAP image)
{
    hImage = image;
    if(getHandle() != 0)
        SendMessage(getHandle(), BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)image);
}

void Button::setImage(const char * fileName)
{
    HBITMAP hBmp;
    hBmp = LoadIMGFromFile(fileName);
    if(hBmp != NULL)
        setImage(hBmp);
}

void Button::setImageResource(const char * resourceName)
{
    HBITMAP hBmp;
    hBmp = LoadIMGResource(GetModuleHandle(NULL), resourceName);
    if(hBmp != NULL)
        setImage(hBmp);
}
