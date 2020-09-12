#ifndef _FONT_H_
#define _FONT_H_
#include <windows.h>
#include "strings.h"

class Control;

class Font
{
private:
    HFONT hFont;
    int size;
    String name;
    Control * owner;
    int quality;
public:
    Font(Control * owner);
    Font(Control * owner, String fontName);
    Font(Control * owner, String fontName, int size);
    ~Font();
    void setSize(int size);
    void setName(String fontName);
    void setQuality(int quality);
    int getSize();
    int getHeight(HDC& hDC);
    int getWidth(HDC& hDC, String text);
    HFONT getHandle();
};

#endif
