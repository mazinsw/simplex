#ifndef _IMAGEUTIL_H_
#define _IMAGEUTIL_H_
#include <windows.h>
#include "strings.h"

void Image_load();
void Image_release();
void DrawImage(HDC hDC, int dstX, int dstY, 
               HBITMAP hBmp, int srcX, int srcY, int srcWidth, int srcHeight);
HBITMAP LoadIMGFromFile(String fileName);
HBITMAP LoadIMGResource(HINSTANCE hInstance, String resname);

#endif
