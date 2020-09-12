#ifndef _IMAGE_H_
#define _IMAGE_H_
#include "control.h"

class Image: public Control
{
private:
    bool autoSize;
    bool center;
    HBITMAP hBmp;
protected:
    void resize(int width, int height);
    void paint(HDC & hdc);
    LRESULT message(UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
    Image();
    ~Image();
    void setAutoSize(bool autoSize);
    void setCenter(bool center);
    bool create(int x, int y, int width, int height, Control * parent);
    bool load(String file);
    bool loadResource(String resource);
};

#endif
