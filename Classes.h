#pragma once
// Global Variables//
static bool needsRedraw = false;
static RECT lastButtonRect = { 0 };

//------------------------------------------------------------------------------//Mouse Class
class Mouse_ {
public:
    float x, y;
    bool L_butt, R_butt;
    void Update() {
        POINT p;
        GetCursorPos(&p);
        ScreenToClient(window.hWnd, &p);
        x = static_cast<float>(p.x);                                                
        y = static_cast<float>(p.y);
        L_butt = (GetAsyncKeyState(VK_LBUTTON));
        R_butt = (GetAsyncKeyState(VK_RBUTTON));
    }
};
Mouse_ Mouse;

//=============================================================================||

class Button {
public:
    float x, y, width, height;
    HBITMAP hBitmap;
    HBITMAP hBitmapGlow;
    bool isDragging = false;
    bool isHovered = false;
    float dragOffsetX = 0;
    float dragOffsetY = 0;

    bool CheckCollisionMouse() {
        return Mouse.x < x + width && Mouse.x > x && Mouse.y < y + height && Mouse.y > y;
    }

    void Load(const char* imagename, const char* imagenameglow, float x_, float y_, float w, float h) {
        x = x_; y = y_;
        hBitmap = LoadBMP(imagename);
        hBitmapGlow = LoadBMP(imagenameglow);
        height = h;
        width = w;
    }

    void StartDragging() {
        if (CheckCollisionMouse() && Mouse.R_butt) {
            isDragging = true;
            dragOffsetX = Mouse.x - x;
            dragOffsetY = Mouse.y - y;
        }
    }

    void UpdateDragging() {
        if (isDragging && Mouse.R_butt) {
            x = Mouse.x - dragOffsetX;
            y = Mouse.y - dragOffsetY;
        }
        else {
            isDragging = false;
        }
    }

   void Show(HDC hdc, bool alpha) {
        // Определяем, какой битмап использовать
        HBITMAP currentBitmap = (isHovered || isDragging) ? hBitmapGlow : hBitmap;
        
        
        HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, currentBitmap);
        BITMAP bm;
        if (hOldBitmap) {
           GetObject(currentBitmap, sizeof(BITMAP), &bm);
            if (alpha) {
                    TransparentBlt(window.context, x, y, width, height, hdcMem, NULL, NULL, width, height, RGB(0, 0, 0));
            }
            else {
                    StretchBlt(window.context, x, y, width, height, hdcMem, NULL, NULL, width, height, SRCCOPY);
            }
            SelectObject(hdcMem, hOldBitmap);
        }
        
        DeleteDC(hdcMem);
    }
};

Button Exit;
HBITMAP Exit_bmp;

//=============================================================================||

struct Player_ { //TOGO
  
};

Player_ player;

//=============================================================================||