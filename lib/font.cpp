#include "font.h"
#include "control.h"

Font::Font(Control * owner)
{
    HDC hDC;
    LOGFONT lf;

    this->owner = owner;
    GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
    name = String(lf.lfFaceName);
    quality = lf.lfQuality;
    hDC = GetDC(0);
    size = - (lf.lfHeight * 72) / GetDeviceCaps(hDC, LOGPIXELSY);
    hFont = CreateFontIndirect(&lf);
}

Font::Font(Control * owner, String fontName)
{
    int nHeight;
    HDC hDC;

    this->owner = owner;
    size = 11;
    name = fontName;
    quality = ANTIALIASED_QUALITY;
    hDC = GetDC(0);
    nHeight = -MulDiv(size, GetDeviceCaps(hDC, LOGPIXELSY), 72);
    hFont = CreateFont(nHeight, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE,
                       DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
                       quality, VARIABLE_PITCH, name.c_str());
}

Font::Font(Control * owner, String fontName, int size)
{
    int nHeight;
    HDC hDC;

    this->owner = owner;
    if(size > 3)
        this->size = size;
    name = fontName;
    quality = ANTIALIASED_QUALITY;
    hDC = GetDC(0);
    nHeight = -MulDiv(this->size, GetDeviceCaps(hDC, LOGPIXELSY), 72);
    hFont = CreateFont(nHeight, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE,
                       DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
                       quality, VARIABLE_PITCH, name.c_str());
}

Font::~Font()
{
    DeleteObject(hFont);
}

void Font::setName(String fontName)
{
    LOGFONT lf;
    HFONT hOldFont;

    if(fontName == name)
        return;
    this->name = fontName;
    GetObject(hFont, sizeof(LOGFONT), &lf);
    strcpy(lf.lfFaceName, name.c_str());
    hOldFont = hFont;
    hFont = CreateFontIndirect(&lf);
    if(owner != NULL)
        SendMessage(owner->getHandle(), WM_SETFONT, (WPARAM)hFont, 0);
    DeleteObject(hOldFont);
}

void Font::setQuality(int quality)
{
    LOGFONT lf;
    HFONT hOldFont;

    if(this->quality == quality)
        return;
    this->quality = quality;
    GetObject(hFont, sizeof(LOGFONT), &lf);
    lf.lfQuality = quality;
    hOldFont = hFont;
    hFont = CreateFontIndirect(&lf);
    if(owner != NULL)
        SendMessage(owner->getHandle(), WM_SETFONT, (WPARAM)hFont, 0);
    DeleteObject(hOldFont);
}

void Font::setSize(int size)
{
    LOGFONT lf;
    HFONT hOldFont;
    int nHeight;
    HDC hDC;

    if(this->size == size)
        return;
    this->size = size;
    GetObject(hFont, sizeof(LOGFONT), &lf);
    hDC = GetDC(0);
    nHeight = -MulDiv(size, GetDeviceCaps(hDC, LOGPIXELSY), 72);
    lf.lfHeight = nHeight;
    hOldFont = hFont;
    hFont = CreateFontIndirect(&lf);
    if(owner != NULL)
        SendMessage(owner->getHandle(), WM_SETFONT, (WPARAM)hFont, 0);
    DeleteObject(hOldFont);
}

int Font::getSize()
{
    return size;
}

int Font::getHeight(HDC& hDC)
{
    SIZE sz;
    const char sample[] = "MY SAMPLE";
    HFONT hOldFont;
    
    hOldFont = (HFONT)SelectObject(hDC, hFont);
    GetTextExtentPoint(hDC, sample, 9, &sz);
    SelectObject(hDC, hOldFont);
    return sz.cy;
}

int Font::getWidth(HDC& hDC, String text)
{
    SIZE sz;
    HFONT hOldFont;
    
    hOldFont = (HFONT)SelectObject(hDC, hFont);
    GetTextExtentPoint(hDC, text.c_str(), text.length(), &sz);
    SelectObject(hDC, hOldFont);
    return sz.cx;
}

HFONT Font::getHandle()
{
    return hFont;
}

