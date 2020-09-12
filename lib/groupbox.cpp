#include "groupbox.h"

bool GroupBox::create(int x, int y, int width, int height, String text,
                      Control* parent)
{
    if(!createControl(0, BS_GROUPBOX|WS_TABSTOP, "BUTTON", text, x, y, width, height, 
        parent))
        return false;
    return false;
}

