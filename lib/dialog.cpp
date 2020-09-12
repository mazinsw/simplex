#include "dialog.h"


DWORD Dialog::getStyle()
{
    return WS_DLGFRAME | WS_CAPTION | WS_SYSMENU | WS_POPUP;
}

int Dialog::showModal()
{
    int result;

    show();
    if(getParent() != NULL)
    {
        getParent()->setEnabled(false);
        SetActiveWindow(getHandle());   
    }
    result = run();
    if(getParent() != NULL)
    {
        getParent()->setEnabled(true);
        SetActiveWindow(getParent()->getHandle());
    }
    hide();
    return result;
}

void Dialog::closeDialog(int result)
{
    terminate(result);
}

