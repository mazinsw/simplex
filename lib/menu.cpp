#include "menu.h"
#include "control.h"
#include <stdio.h>

static int m_command = 1000;


Menu::Menu()
{
    items = new MenuItem(this);
}

Menu::~Menu()
{
    delete items;
}

LRESULT Menu::menuProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_COMMAND:
    {
        Control * control = (Control*)GetWindowLong(hWnd, GWL_USERDATA);
        MenuItem * item = getItems().find(LOWORD(wParam));
        if(item != NULL && control != NULL)
        {
            std::list<ClickEvent*>::iterator ite;
            ClickEvent * click;

            for(ite = clickEvent.begin(); ite != clickEvent.end(); ite++)
            {
                click = *ite;
                click->click(*item);
            }
        }
        break;
    }
    }
    return 0;
}

void Menu::addClickEvent(ClickEvent* click)
{
    clickEvent.push_back(click);
}

String Menu::getClassName()
{
    return "Menu";
}

MenuItem& Menu::getItems()
{
    return *items;
}

HMENU Menu::getHandle()
{
    return items->getHandle();
}

String PopupMenu::getClassName()
{
    return "PopupMenu";
}

String MainMenu::getClassName()
{
    return "MainMenu";
}

MenuItem::MenuItem(Object * parent)
{
    this->parent = (MenuItem*)parent;
    hMenu = 0;
    command = 0;
}

MenuItem::MenuItem(Object * parent, String caption)
{
    this->parent = (MenuItem*)parent;
    this->caption = caption;
    command = 0;
    hMenu = 0;
}

MenuItem::~MenuItem()
{
    if(hMenu != 0)
        DestroyMenu(hMenu);
}

String MenuItem::getClassName()
{
    return "MenuItem";
}
void MenuItem::deleteAll()
{
    int i;

    i = GetMenuItemCount(getHandle());
    while(i > 0)
    {
        RemoveMenu(getHandle(), i - 1, MF_BYPOSITION);
        i--;
    }
    //DestroyMenu(hMenu);
    //hMenu = 0;
}

void MenuItem::recreate()
{
    int flags, i;

    deleteAll();
    for(i = 0; i < count(); i++)
    {
        flags = MF_BYPOSITION;

        if(get(i)->caption == "-")
            flags |= MF_SEPARATOR;
        else
            flags |= MF_STRING;
        if(get(i)->count() > 0)
        {
            InsertMenu(getHandle(), (DWORD) - 1, MF_POPUP | flags,
                       (UINT)get(i)->getHandle(), get(i)->caption.c_str());
            get(i)->recreate();
        }
        else
        {
            get(i)->command = m_command++;
            InsertMenu(getHandle(), (DWORD) - 1, flags, (UINT)get(i)->command,
                       get(i)->caption.c_str());
        }
    }
}

void MenuItem::add(MenuItem * item)
{
    items.push_back(item);
    recreate();
}

void MenuItem::remove(MenuItem* item)
{
    //TODO Generated function
}

MenuItem* MenuItem::find(int id)
{
    int i;
    MenuItem* res;

    for(i = 0; i < count(); i++)
    {
        if(get(i)->count() > 0)
            res = get(i)->find(id);
        else if(id == get(i)->command)
            return get(i);
        if(res != NULL)
            return res;
    }
    return NULL;
}

void MenuItem::erase(int index)
{
    //TODO Generated function
}

String MenuItem::getCaption()
{
    return caption;
}

MenuItem* MenuItem::get(int index)
{
    return items[index];
}

int MenuItem::count()
{
    return items.size();
}

HMENU MenuItem::getHandle()
{
    if(hMenu == 0)
    {
        if(parent->getClassName() == "Menu" ||
                parent->getClassName() == "MainMenu")
            hMenu = CreateMenu();
        else
            hMenu = CreatePopupMenu();
    }
    return hMenu;
}

