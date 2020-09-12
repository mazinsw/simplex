#ifndef _MENU_H_
#define _MENU_H_
#include <windows.h>
#include <vector>
#include "strings.h"
#include <list>
#include "object.h"
#include "event.h"

class MenuItem;

class Menu: public Object
{
private:
    MenuItem * items;
    std::list<ClickEvent*> clickEvent;
public:
    Menu();
    ~Menu();
    String getClassName();
    void addClickEvent(ClickEvent* click);
    MenuItem& getItems();
    HMENU getHandle();
    LRESULT menuProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

class PopupMenu: public Menu
{
public:
    String getClassName();
};

class MainMenu: public Menu
{
public:
    String getClassName();
};

class MenuItem: public Object
{
private:
    std::vector<MenuItem*> items;
    Menu * menu;
    String caption;
    MenuItem * parent;
    HMENU hMenu;
    int command;
    void deleteAll();
    void recreate();
public:
    MenuItem(Object * parent);
    MenuItem(Object * parent, String caption);
    ~MenuItem();
    String getClassName();
    String getCaption();
    void add(MenuItem * item);
    void remove(MenuItem* item);
    MenuItem* find(int id);
    void erase(int index);
    MenuItem* get(int index);
    int count();
    HMENU getHandle();
};


#endif
