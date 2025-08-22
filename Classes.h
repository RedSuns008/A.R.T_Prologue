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

//class Button {
//public:
//
//    float x, y, width, height;
//    HBITMAP hBitmap;
//    HBITMAP hBitmapGlow;
//
//    bool CheckCollisionMouse() {
//        return Mouse.x < x + width && Mouse.x > x && Mouse.y < y + height && Mouse.y > y;
//    }
//    void Load(const char* imagename, const char* imagenameglow, float x_, float y_, float w, float h) {
//        x = x_; y = y_;
//        hBitmap = LoadBMP(imagename);
//        hBitmapGlow = LoadBMP(imagenameglow);
//        height = h; // *window.height
//        width = w; // *window.width;
//        x = x;  // window.width / 2 - width *
//        y = y;  // window.height / 2 + height*
//    }
//
//    bool Show(int x, int y) {
//        bool pw_collision = CheckCollisionMouse();
//
//        ShowBitmap(x, y, width, height, pw_collision ? hBitmapGlow : hBitmap, false);
//        return pw_collision;
//    }
//    bool Mouse_Move() {
//        while (CheckCollisionMouse() && Mouse.L_butt) {
//            x = Mouse.x;
//            y = Mouse.y;
//        }
//        return 1;
//    }
//};


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
   void Show(HDC hdc) {
        // Определяем, какой битмап использовать
        HBITMAP currentBitmap = (isHovered || isDragging) ? hBitmapGlow : hBitmap;
        
        // Отрисовываем кнопку
        HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, currentBitmap);
        
        BITMAP bm;
        GetObject(currentBitmap, sizeof(BITMAP), &bm);
        
        // Используем TransparentBlt для поддержки прозрачности, если нужно
        TransparentBlt(hdc, x, y, width, height,
                      hdcMem, 0, 0, bm.bmWidth, bm.bmHeight,
                      RGB(255, 0, 255)); // Цвет, который должен быть прозрачным
        
        SelectObject(hdcMem, hOldBitmap);
        DeleteDC(hdcMem);
    }
};








Button Exit;
HBITMAP Exit_bmp;

struct Player_ { //TOGO
  
};

Player_ player;