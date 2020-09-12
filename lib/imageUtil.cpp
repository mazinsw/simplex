#include "imageUtil.h"
#include <loadimg.h>

typedef BOOL (WINAPI ALPHABLENDFUNC)(HDC, int, int, int, int,
                                     HDC, int, int, int, int,
                                     BLENDFUNCTION);
static ALPHABLENDFUNC* pAlphaBlend = NULL;
static HMODULE msimg = NULL;
static unsigned int loadCount = 0;

void DrawImage(HDC hDC, int dstX, int dstY, 
               HBITMAP hBmp, int srcX, int srcY, int srcWidth, int srcHeight)
{
    HDC hMemDC, hOldMemDC;
    BITMAP bm;
    BLENDFUNCTION bfn;

    GetObject(hBmp, sizeof(BITMAP), &bm);
    hMemDC = CreateCompatibleDC(hDC);
    hOldMemDC = (HDC)SelectObject(hMemDC, hBmp);
    if(bm.bmBitsPixel == 32)
    {
        Image_load();

        // Struct with info for AlphaBlend
        bfn.BlendOp = AC_SRC_OVER;
        bfn.BlendFlags = 0;
        bfn.SourceConstantAlpha = 0xFF;
        bfn.AlphaFormat = AC_SRC_ALPHA;
        pAlphaBlend(hDC, dstX, dstY, srcWidth, srcHeight,
                    hMemDC, srcX, srcY, srcWidth, srcHeight,
                    bfn); // Display bitmap
    }
    else
        BitBlt(hDC, dstX, dstY, srcWidth, srcHeight, hMemDC, srcX, srcY, SRCCOPY);
    SelectObject(hOldMemDC, hBmp);
    DeleteDC(hMemDC);
}

void Image_load()
{
    if(loadCount > 0)
        return;
    msimg = LoadLibrary("msimg32.dll");
    if(msimg == NULL)
    {
        MessageBox(0, "Can't load msimg32.dll.", "Image", MB_ICONERROR | MB_OK);
        return;
    }
    pAlphaBlend = (ALPHABLENDFUNC*)GetProcAddress(msimg, "AlphaBlend");
    loadCount++;
}

void Image_release()
{
    if(loadCount == 0)
        return;
    FreeLibrary(msimg);
    loadCount = 0;
}

static HBITMAP ImgInfoToBmp(ImgInfo * img)
{
    int x, y;
    HBITMAP hBmp = NULL;
    BYTE *pvPixels;
    HDC hDC;
    BITMAPINFO bmi;
    unsigned char * ptr;
    double factor;
                        
    hDC = CreateCompatibleDC(NULL);
    ZeroMemory(&bmi, sizeof(BITMAPINFO));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = img->w;
    bmi.bmiHeader.biHeight = img->h;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = img->colors * 8;
    hBmp = CreateDIBSection(hDC, (BITMAPINFO*)&bmi, DIB_RGB_COLORS,
                            (void **)&pvPixels, NULL, 0);
    if(hBmp == NULL)
        return NULL;
    SelectObject(hDC, hBmp);

    for(y = 0; y < img->h; y++)
    {
        ptr = img->pbytes + img->w *img->colors* (img->h - y - 1);
        for(x = 0; x < img->w; x++)
        {
            switch(img->colors)
            {
            case 1:
                pvPixels[0] = ptr[0];
                break;
            case 3:
                pvPixels[0] = ptr[2];
                pvPixels[1] = ptr[1];
                pvPixels[2] = ptr[0];
                break;
            case 4:
                factor = ptr[3] / 255.0;
                pvPixels[0] = (BYTE)(ptr[2] * factor);
                pvPixels[1] = (BYTE)(ptr[1] * factor);
                pvPixels[2] = (BYTE)(ptr[0] * factor);
                pvPixels[3] = ptr[3];
                break;
            }
            pvPixels += img->colors;
            ptr += img->colors;
        }
        pvPixels += (img->w *img->colors) % 4;
    }
    DeleteDC(hDC);
    return hBmp;
}

HBITMAP LoadIMGFromFile(String fileName)
{
    HBITMAP hBmp;
    ImgInfo * img;

    img = load_image_from_file(fileName.c_str());
    if(img == NULL)
        return false;
    hBmp = ImgInfoToBmp(img);    
    free_image(img);
    return hBmp;
}

HBITMAP LoadIMGResource(HINSTANCE hInstance, String resname)
{
    HBITMAP hBitmap;
    ImgInfo * img;
    HRSRC hResource = FindResource(hInstance, resname.c_str(), RT_RCDATA);
    if (!hResource)
        return NULL;
    DWORD imageSize = SizeofResource(hInstance, hResource);
    if (!imageSize)
        return NULL;
    const void* pResourceData = LockResource(LoadResource(hInstance, hResource));
    if (!pResourceData)
        return NULL;
    void* m_hBuffer  = GlobalAlloc(GMEM_MOVEABLE, imageSize);
    if (!m_hBuffer)
        return NULL;
    void* pBuffer = GlobalLock(m_hBuffer);
    if (!pBuffer)
    {
        GlobalFree(m_hBuffer);
        return NULL;
    }
    CopyMemory(pBuffer, pResourceData, imageSize);
    img = load_image_from_bytes((unsigned char *)pBuffer, imageSize);
    GlobalUnlock(m_hBuffer);
    GlobalFree(m_hBuffer);
    if(img == NULL)
        return NULL;
    hBitmap = ImgInfoToBmp(img);
    free_image(img);
    return hBitmap;
}
