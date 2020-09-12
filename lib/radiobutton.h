#ifndef _RADIOBUTTON_H_
#define _RADIOBUTTON_H_
#include "button.h"

class RadioButton: public Button
{
protected:
    DWORD getStyle();
    bool checked;
public:
    RadioButton();
    bool isChecked();
    bool create(int x, int y, int width, int height,
                String text, Control * parent);
    void setChecked(bool checked);
};

#endif

