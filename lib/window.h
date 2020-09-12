#ifndef _WINDOW_H_
#define _WINDOW_H_
#include "control.h"

#define BUTTON_MINIMIZE 1
#define BUTTON_MAXIMIZE 2
#define BUTTON_CLOSE    4
#define BUTTON_ALL      7

class Window: public Control
{
private:
    bool done;
    bool resizable;
    MainMenu * menu;
protected:
    virtual DWORD getStyle();
    LRESULT message(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void onClose(bool &canClose) {}
    virtual void move(int x, int y) {}
public:
    Window();
    ~Window();
    void close();
    void setResizable(bool enable);
    void setIcon(String resource);
    void setButtonEnabled(int button, bool enable);
    void showMessage(String text);
    void setMenu(MainMenu * menu);
    void terminate(int result);
    bool create(String className, int width, int height,
                String title, Control * parent);
    int messageBox(String text, String title, UINT type);
    int run();
};

#endif

