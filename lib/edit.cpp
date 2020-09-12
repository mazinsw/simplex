#include "edit.h"

Edit::Edit()
{
    readOnly = false;
}

bool Edit::create(int x, int y, int width, int height,
                  String text, Control * parent)
{
    if(!createControl(WS_EX_CLIENTEDGE, WS_TABSTOP | ES_AUTOHSCROLL, "Edit",
                      text, x, y, width, height, parent))
        return false;
    if(readOnly)
        setReadOnly(readOnly);
    return true;
}

void Edit::addChangeEvent(ChangeEvent* change)
{
    changeEvent.push_back(change);
}

void Edit::setPasswordChar(char ch)
{
    if(getHandle() != 0)
        SendMessage(getHandle(), EM_SETPASSWORDCHAR, ch, 0);
}

void Edit::command(WPARAM wParam, LPARAM lParam)
{
    switch(HIWORD(wParam))
    {
    case EN_CHANGE:
    {
        std::list<ChangeEvent*>::iterator ite;
        ChangeEvent * change;

        for(ite = changeEvent.begin(); ite != changeEvent.end(); ite++)
        {
            change = *ite;
            change->change(*this);
        }
        break;
    }
    }
    Control::command(wParam, lParam);
}

void Edit::selectAll()
{
    if(getHandle() != 0)
        SendMessage(getHandle(), EM_SETSEL, 0, -1);
}

void Edit::setSelStart(int start)
{
    int end;

    SendMessage(getHandle(), EM_GETSEL, 0, (LPARAM)&end);
    if(end < start)
        end = start;
    SendMessage(getHandle(), EM_SETSEL, start, end);
}

void Edit::setSelLength(int length)
{
    int start;

    if(getHandle() != 0)
    {
        SendMessage(getHandle(), EM_GETSEL, (WPARAM)&start, 0);
        SendMessage(getHandle(), EM_SETSEL, start, start + length);
    }
}

int Edit::getSelLength()
{
    int start = 0, end = 0;

    if(getHandle() != 0)
        SendMessage(getHandle(), EM_GETSEL, (WPARAM)&start, (LPARAM)&end);
    return end - start;
}

void Edit::setSelText(String text)
{
    if(getHandle() != 0)
        SendMessage(getHandle(), EM_REPLACESEL, (WPARAM)text.c_str(), text.length());
}

void Edit::setReadOnly(bool readOnly)
{
    this->readOnly = readOnly;
    if(getHandle() != 0)
        SendMessage(getHandle(), EM_SETREADONLY, (WPARAM)(readOnly) ? TRUE : FALSE, 0);
}

bool Edit::isReadOnly()
{
    return readOnly;
}

void Edit::setLimitText(int length)
{
    if(getHandle() != 0)
        SendMessage(getHandle(), EM_SETLIMITTEXT, length, 0);
}

void Edit::clear()
{
    setText("");
}

