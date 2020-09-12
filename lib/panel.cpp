#include "panel.h"

Panel::Panel()
{
    alignment = NONE_ALIGN;
}

Panel::~Panel()
{
    if(getParent() != NULL)
        getParent()->delResizeEvent(this);
}

void Panel::updateBounds()
{
    RECT rect, parentRect;

    if(alignment == NONE_ALIGN || getParent() == NULL)
        return;
    getClientRect(rect);
    getParent()->getClientRect(parentRect);
    if(alignment == CLIENT_ALIGN)
    {
        rect.left = parentRect.left;
        rect.top = parentRect.top;
        rect.right = parentRect.right;
        rect.bottom = parentRect.bottom;
    }
    SetWindowPos(getHandle(), 0, rect.left, rect.top,
                 rect.right - rect.left, rect.bottom - rect.top, 0);
}

void Panel::setAlign(Align alignment)
{
    this->alignment = alignment;
    updateBounds();
}

bool Panel::create(int x, int y, int width, int height, Control* parent)
{
    if(!registerClass("Panel"))
       return false;
    if(!createControl(WS_EX_CONTROLPARENT, WS_VISIBLE| WS_GROUP, "Panel", "", x, y, width, height, parent))
        return false;
    if(parent != NULL)
        parent->addResizeEvent(this);
    return true;
}

void Panel::resize(Object & sender, int width, int height)
{
    updateBounds();
}

