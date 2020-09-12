#include "image.h"
#include "imageUtil.h"

Image::Image()
{
    autoSize = true;
    center = true;
    hBmp = 0;
}

Image::~Image()
{
    if(hBmp != NULL)
        DeleteObject(hBmp);
}

void Image::setAutoSize(bool autoSize)
{
    if(this->autoSize == autoSize)
        return;
    this->autoSize = autoSize;
    if(getHandle() != 0)
    {
        resize(getClientWidth(), getClientHeight());
        invalidate();
    }
}

void Image::setCenter(bool center)
{
    if(this->center == center)
        return;
    this->center = center;
    if(getHandle() != 0)
        invalidate();
}

void Image::paint(HDC & hdc)
{
    HDC hBffDC, hOldBffDC;
    HBITMAP hBff;
    BITMAP bm;
    int xDest = 0, yDest = 0;
    RECT rect;

    GetObject(hBmp, sizeof(BITMAP), &bm);
    hBffDC = CreateCompatibleDC(hdc);
    hBff = CreateCompatibleBitmap(hdc, bm.bmWidth, bm.bmHeight);
    hOldBffDC = (HDC)SelectObject(hBffDC, hBff);
    rect.left = 0;
    rect.top = 0;
    rect.right = bm.bmWidth;
    rect.bottom = bm.bmHeight;
    FillRect(hBffDC, &rect, GetSysColorBrush(COLOR_BTNFACE));
    if(center)
    {
        xDest = (getClientWidth() - bm.bmWidth) / 2;
        yDest = (getClientHeight() - bm.bmHeight) / 2;
    }
    DrawImage(hBffDC, 0, 0, hBmp, 0, 0, bm.bmWidth, bm.bmHeight);
    BitBlt(hdc, xDest, yDest, bm.bmWidth, bm.bmHeight, hBffDC, 0, 0, SRCCOPY);
    SelectObject(hOldBffDC, hBff);
    DeleteDC(hBffDC);
    DeleteObject(hBff);
}

bool Image::create(int x, int y, int width, int height, Control* parent)
{
    BITMAP bm;

    if(!registerClass("Image"))
        return false;
    if(hBmp != NULL && autoSize)
    {
        GetObject(hBmp, sizeof(BITMAP), &bm);
        width = bm.bmWidth;
        height = bm.bmHeight;
    }
    if(!createControl(0, 0, "Image", "", x, y, width, height, parent))
        return false;
    if(parent != NULL)
        show();
    return true;
}

bool Image::load(String file)
{
    HBITMAP hBmpTemp;
    BITMAP bm;

    hBmpTemp = LoadIMGFromFile(file.c_str());
    if(hBmpTemp == NULL)
        return false;
    if(hBmp != NULL)
        DeleteObject(hBmp);
    hBmp = hBmpTemp;
    if(getHandle() != 0)
    {
        invalidate();
        if(autoSize)
        {
            GetObject(hBmp, sizeof(BITMAP), &bm);
            setSize(bm.bmWidth, bm.bmHeight);
        }
    }
    return true;
}

bool Image::loadResource(String resource)
{
    HBITMAP hBmpTemp;
    BITMAP bm;

    hBmpTemp = LoadIMGResource(GetModuleHandle(NULL), resource.c_str());
    if(hBmpTemp == NULL)
        return false;
    if(hBmp != NULL)
        DeleteObject(hBmp);
    hBmp = hBmpTemp;
    if(getHandle() != 0)
    {
        invalidate();
        if(autoSize)
        {
            GetObject(hBmp, sizeof(BITMAP), &bm);
            setSize(bm.bmWidth, bm.bmHeight);
        }
    }
    return true;
}

void Image::resize(int width, int height)
{
    BITMAP bm;
    
    if(autoSize)
    {
        GetObject(hBmp, sizeof(BITMAP), &bm);
        setSize(bm.bmWidth, bm.bmHeight);
    }
}

LRESULT Image::message(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if(uMsg == WM_ERASEBKGND)
        return 0;
    if(uMsg == WM_PAINT)
    {
        PAINTSTRUCT ps;
        HDC hdc;
        
        hdc = BeginPaint(getHandle(), &ps);
        paint(hdc);
        EndPaint(getHandle(), &ps);
        return 0;
    }
    return Control::message(uMsg, wParam, lParam);
}

