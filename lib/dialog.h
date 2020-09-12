#ifndef _DIALOG_H_
#define _DIALOG_H_
#include "window.h"

class Dialog: public Window
{
protected:
    DWORD getStyle();
public:
    int showModal();
    void closeDialog(int result);
};

#endif

