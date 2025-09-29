#pragma once
// Global Variables//
static bool needsRedraw = false;


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
        if (CheckCollisionMouse()) {
            isDragging = true;
            dragOffsetX = Mouse.x - x;
            dragOffsetY = Mouse.y - y;
            // Устанавливаем захват мыши сразу
            SetCapture(window.hWnd);
        }
    }

    void UpdateDragging() {
        if (isDragging) {
            // Используем уже обновленные координаты из структуры Mouse
            x = Mouse.x - dragOffsetX;
            y = Mouse.y - dragOffsetY;

            // Ограничиваем движение кнопки границами экрана
            if (x < 0) x = 0;
            if (y < 0) y = 0;
            if (x > window.width - width) x = window.width - width;
            if (y > window.height - height) y = window.height - height;
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
                    TransparentBlt(hdc, x, y, width, height, hdcMem, NULL, NULL, width, height, RGB(0, 0, 0));
            }
            else {
                    StretchBlt(hdc, x, y, width, height, hdcMem, NULL, NULL, width, height, SRCCOPY);
            }
            SelectObject(hdcMem, hOldBitmap);
        }
        
        DeleteDC(hdcMem);
   }
};

Button Exit, Inventory;
HBITMAP Exit_bmp, Inventory_bmp;

//=============================================================================||


#include <cmath>
struct Player_ {
    int x, y, width, height, speed, prevX, prevY;
    HBITMAP hBitmap;
    bool isMoving = false;
    
    bool CheckCollisionMouse() {
        return Mouse.x < x + width && Mouse.x > x && Mouse.y < y + height && Mouse.y > y;
    }
  
    void Load(const char* imagename, int x_, int y_, int w, int h, int s) {
        x = x_;
        y = y_;
        hBitmap = LoadBMP(imagename);
        height = h;
        width = w;
        speed = s;
    }

    void Show(HDC hdc) {
        ShowBitmap(hdc,x, y, width, height, hBitmap, true);
    }
  

    void ProcessInput() {
        prevX = x;
        prevY = y;
        // Вектор движения
        int moveX = 0;
        int moveY = 0;

        // Проверяем все клавиши независимо
        if (GetAsyncKeyState('W') & 0x8000) moveY -= speed;
        if (GetAsyncKeyState('S') & 0x8000) moveY += speed;
        if (GetAsyncKeyState('A') & 0x8000) moveX -= speed;
        if (GetAsyncKeyState('D') & 0x8000) moveX += speed;

        // Применяем движение, если есть хотя бы одно направление
        if (moveX != 0 || moveY != 0) {
            x += moveX;
            y += moveY;
            isMoving = true;

            // Ограничение движения по границам экрана
            if (x < 0) x = 0;
            if (y < 0) y = 0;
            if (x > window.width - width) x = window.width - width;
            if (y > window.height - height) y = window.height - height;
        }
        else {
            isMoving = false;
        }
    }
public:
    RECT GetRect() const {
        return { x, y, x + width, y + height };
    }

    RECT GetPrevRect() const {
        return { prevX, prevY, prevX + width, prevY + height };
    }

    // Дополнительный метод для обработки столкновений с другими объектами
    bool CheckCollision(const Player_ & other) {
        return x < other.x + other.width &&
            x + width > other.x &&
            y < other.y + other.height &&
            y + height > other.y;
    }

   

};



Player_ player;
HBITMAP player_bmp;
//=============================================================================||