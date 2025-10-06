#pragma once
#include <windows.h>
//------------------------------------------------------------------------------//Структура окна
struct Window {
    HWND hWnd;
    HDC device_context;
    HDC context;
    int width, height;
};
Window window;
//------------------------------------------------------------------------------//


void ShowBitmap(HDC hdc, int x, int y, int width, int height, HBITMAP hBitmap, bool alpha) {

        HDC hMemDC = CreateCompatibleDC(hdc);
        HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
        BITMAP bm;

        if (hOldBitmap) {
            GetObject(hBitmap, sizeof(HBITMAP), &bm);
            if (alpha) {
                TransparentBlt(hdc, x, y, width, height, hMemDC, NULL, NULL, width, height, RGB(0, 0, 0));
            }
            else {
                StretchBlt(hdc, x, y, width, height, hMemDC, NULL, NULL, width, height, SRCCOPY);
            }
            SelectObject(hMemDC, hOldBitmap);
        }
        DeleteDC(hMemDC);
    }



HBITMAP LoadBMP(const char* name)
{
    return (HBITMAP)LoadImageA(NULL, name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}