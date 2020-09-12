#ifndef _EDIT_H_
#define _EDIT_H_
#include "control.h"

class Edit: public Control
{
private:
    std::list<ChangeEvent*> changeEvent;
    bool readOnly;
protected:
    void command(WPARAM wParam, LPARAM lParam);
public:
    Edit();
    bool create(int x, int y, int width, int height,
                String text, Control * parent);
    void setPasswordChar(char ch);
    void addChangeEvent(ChangeEvent* change);
    void clear();
    void selectAll();
    void setSelStart(int start);
    void setSelLength(int length);
    int  getSelLength();
    void setSelText(String text);
    void setReadOnly(bool readOnly);
    bool isReadOnly();
    void setLimitText(int length);
};

#endif

