#ifndef _PANEL_H_
#define _PANEL_H_
#include "control.h"

enum Align
{
    NONE_ALIGN,
    LEFT_ALIGN,
    RIGHT_ALIGN,
    TOP_ALIGN,
    BOTTOM_ALIGN,
    CLIENT_ALIGN
};

class Panel: public Control, ResizeEvent
{
private:
    Align alignment;
    void updateBounds();
    void resize(Object & sender, int width, int height);
public:
    Panel();
    ~Panel();
    void setAlign(Align alignment);
    bool create(int x, int y, int width, int height, Control * parent);
};
#endif
