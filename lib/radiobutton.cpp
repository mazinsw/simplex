#include "radiobutton.h"

DWORD RadioButton::getStyle()
{
    return BS_AUTORADIOBUTTON|WS_TABSTOP;
}

RadioButton::RadioButton()
{
    checked = false;
}

bool RadioButton::create(int x, int y, int width, int height,
                String text, Control * parent)
{
    if(Button::create(x, y, width, height, text, parent))
    {
        if(checked)
            setChecked(true);
        return true;
    }
    return false;
}

bool RadioButton::isChecked()
{
    return SendMessage(getHandle(), BM_GETCHECK, 0, 0) == BST_CHECKED;
}

void RadioButton::setChecked(bool checked)
{
    this->checked = checked;
    if(getHandle() != 0)
        SendMessage(getHandle(), BM_SETCHECK, 
            (checked? BST_CHECKED: BST_UNCHECKED), 0);
}

