#pragma once

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
//------------------------------------------------------------------------------//

class Button {
public:

    float x, y, width, height;
    HBITMAP hBitmap;
    HBITMAP hBitmapGlow;

    bool CheckCollisionMouse() {
        return Mouse.x < x + width && Mouse.x > x && Mouse.y < y + height && Mouse.y > y;
    }
    void Load(const char* imagename, const char* imagenameglow, float x_, float y_, float w, float h) {
        x = x_; y = y_;
        hBitmap = LoadBMP(imagename);
        hBitmapGlow = LoadBMP(imagenameglow);
        height = h; // *window.height
        width = w; // *window.width;
        x = x;  // window.width / 2 - width *
        y = y;  // window.height / 2 + height*fdsfdsfs
    }

    bool Show() {
        bool pw_collision = CheckCollisionMouse();

        ShowBitmap(x, y, width, height, pw_collision ? hBitmapGlow : hBitmap, false);
        return pw_collision;
    }
    bool Mouse_Move() {
        if (CheckCollisionMouse() && Mouse.R_butt) {
            x = Mouse.x;
            y = Mouse.y;
        }
        return 1;
    }
};

Button Exit;
HBITMAP Exit_bmp;