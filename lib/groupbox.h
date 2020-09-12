#ifndef _GROUPBOX_H_
#define _GROUPBOX_H_
#include "control.h"

class GroupBox: public Control
{
public:
    bool create(int x, int y, int width, int height,
                String text, Control * parent);
};

#endif
