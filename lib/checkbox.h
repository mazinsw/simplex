#ifndef _CHECKBOX_H_
#define _CHECKBOX_H_
#include "radiobutton.h"

class CheckBox: public RadioButton
{
protected:
    DWORD getStyle();
};

#endif
