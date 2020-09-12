#ifndef _BUTTON_H_
#define _BUTTON_H_
#include "control.h"

class Button: public Control
{
private:
    std::list<ClickEvent*> clickEvent;
    HBITMAP hImage;
    bool showCaption;
protected:
    virtual DWORD getStyle();
    void command(WPARAM wParam, LPARAM lParam);
public:
    Button();
    bool create(int x, int y, int width, int height,
                String text, Control * parent);
    void addClickEvent(ClickEvent* click);
    void setImage(HBITMAP image);
    void setImage(const char * fileName);
    void setImageResource(const char * resourceName);
    void setShowCaption(bool show);
};

#endif

