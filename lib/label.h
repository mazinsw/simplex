#ifndef _LABEL_H_
#define _LABEL_H_
#include "control.h"

class Label: public Control
{
private:
    bool autoSize;
protected:
    LRESULT message(UINT uMsg, WPARAM wParam, LPARAM lParam);
    void resize(int width, int height);
public:
    Label();
    bool create(int x, int y, int width, int height,
                String text, Control * parent);
};

#endif
