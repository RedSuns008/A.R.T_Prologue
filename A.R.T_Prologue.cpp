//------------------------------------------------------------------------------//
#pragma comment(lib, "Msimg32.lib")
#include <string>
#include <sstream>
#include "framework.h"
#include"cstdio"
#include "A.R.T_Prologue.h"
#define MAX_LOADSTRING 100
//------------------------------------------------------------------------------//
#include "utilities.h"
#include "Classes.h"
//------------------------------------------------------------------------------//

//------------------------------------------------------------------------------//

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HBITMAP BackGround_bmp;
static int counter = 0;
char debugMsg[100];
static UINT_PTR gameTimerId = 0;
//------------------------------------------------------------------------------//


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
//------------------------------------------------------------------------------//

//void ShowInventory() {
//    //if (Inventory.CheckCollisionMouse()) {
//        //ShowBitmap(Inventory.x, Inventory.y, Inventory.width, Inventory.height, Inventory.hBitmap, true);
//    ShowBitmap(1500, 1000, 100, 100, Inventory.hBitmap, true);
//
//    //}
//}

void InitMainMenu() {
    HDC hdcScreen = GetDC(window.hWnd);
    window.device_context = CreateCompatibleDC(hdcScreen); // хендл контекста устройтсва для рисования
    window.width = GetSystemMetrics(SM_CXSCREEN);
    window.height = GetSystemMetrics(SM_CYSCREEN);

   
    player.speed = 20;
    player.x = window.width / 2;
    player.y = window.height / 2;
    player.Load("exit_butt.bmp", player.x, player.y, 50, 50, 10);
    
    Exit.Load("exit_butt.bmp", "exit_butt_glow.bmp", 100, 100, 150, 50);
    Inventory.Load("exit_butt.bmp", "exit_butt_glow.bmp", 1500, 1000, 100, 100);
    BackGround_bmp = (HBITMAP)LoadImageA(NULL, "phon1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);


}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int  nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
  
   InitMainMenu();


    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ARTPROLOGUE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ARTPROLOGUE));

    MSG msg;
    bool isRunning = true;

    while (isRunning) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                isRunning = false;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            // Неблокирующая обработка ввода
            Mouse.Update();
            player.ProcessInput();

            // Перерисовка при изменениях
            bool needsRedraw = false;

            if (player.isMoving) {
                RECT prevRect = player.GetPrevRect();
                RECT currentRect = player.GetRect();
                InvalidateRect(window.hWnd, &prevRect, FALSE);
                InvalidateRect(window.hWnd, &currentRect, FALSE);
                needsRedraw = true;
            }

            if (Exit.isDragging) {
                Exit.UpdateDragging();
                RECT buttonRect = { (LONG)Exit.x, (LONG)Exit.y,
                                  (LONG)(Exit.x + Exit.width),
                                  (LONG)(Exit.y + Exit.height) };
                InvalidateRect(window.hWnd, &buttonRect, FALSE);
                needsRedraw = true;
            }

            // Обновление hover состояния
            static bool wasHovered = false;
            bool isHovered = Exit.CheckCollisionMouse();
            if (wasHovered != isHovered) {
                Exit.isHovered = isHovered;
                RECT buttonRect = { (LONG)Exit.x, (LONG)Exit.y,
                                  (LONG)(Exit.x + Exit.width),
                                  (LONG)(Exit.y + Exit.height) };
                InvalidateRect(window.hWnd, &buttonRect, FALSE);
                wasHovered = isHovered;
                needsRedraw = true;
            }

            if (needsRedraw) {
                UpdateWindow(window.hWnd);
            }

            Sleep(1); // Снижаем нагрузку на CPU
        }
    }
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ARTPROLOGUE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+3);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   window.hWnd = CreateWindowW(szWindowClass, L"AnAl SeX <3", WS_POPUP,
      CW_USEDEFAULT, CW_USEDEFAULT, window.width, window.height,  nullptr, nullptr, hInstance, nullptr);

   if (!window.hWnd)
   {
      return FALSE;
   }

   ShowWindow(window.hWnd, nCmdShow);
   UpdateWindow(window.hWnd);
   gameTimerId = SetTimer(window.hWnd, 1, 5, NULL);

   return TRUE;
}


//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return

// TODO
void DrawBackground(HDC hdc, int width, int height, HBITMAP hBitmap)
{
    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);

    BITMAP bm;
    GetObject(hBitmap, sizeof(BITMAP), &bm);

    StretchBlt(hdc, 0, 0, width, height, hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

    SelectObject(hdcMem, hOldBitmap);
    DeleteDC(hdcMem);

}   


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_TIMER:

      
            // Игровой таймер - обрабатываем ввод и перерисовываем
            player.ProcessInput();

            // Если игрок движется ИЛИ кнопка перетаскивается, перерисовываем
            if (player.isMoving || Exit.isDragging) {
                InvalidateRect(hWnd, NULL, FALSE);
                UpdateWindow(hWnd);
            }
        
        break;

    case WM_RBUTTONDOWN:
        Mouse.x = (float)LOWORD(lParam);
        Mouse.y = (float)HIWORD(lParam);
        Mouse.R_butt = true;
        Exit.StartDragging();
        break;

    case WM_MOUSEMOVE:
    {
        POINT pt = { LOWORD(lParam), HIWORD(lParam) };
        Mouse.x = (float)pt.x;
        Mouse.y = (float)pt.y;


        break; 
    }

    case WM_RBUTTONUP:  
        Mouse.x = (float)LOWORD(lParam);
        Mouse.y = (float)HIWORD(lParam);
        Mouse.R_butt = false;

        if (Exit.isDragging) {
            Exit.isDragging = false;
            ReleaseCapture();
            InvalidateRect(hWnd, NULL, FALSE);
        }
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        window.device_context = BeginPaint(hWnd, &ps);


        window.context = CreateCompatibleDC(window.device_context); // Второй буфер       
        HBITMAP hbmMem = CreateCompatibleBitmap(window.device_context, window.width, window.height);
        HBITMAP hbmOld = (HBITMAP)SelectObject(window.context, hbmMem);
      

        //RECT buttonRect = {(LONG)Exit.x,(LONG)Exit.y,(LONG)(Exit.x + Exit.width),(LONG)(Exit.y + Exit.height)};


        DrawBackground(window.context, window.width, window.height, BackGround_bmp);
        player.Show(window.context);
        Exit.Show(window.context, false);


        //BitBlt(window.device_context, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top, window.context, ps.rcPaint.left, ps.rcPaint.top, SRCCOPY);
       BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);
        
        
        // Очистка
        SelectObject(window.context, hbmOld);
        DeleteObject(hbmMem);
        DeleteDC(window.context);

        EndPaint(hWnd, &ps);
    }
    break;
  
    case WM_COMMAND:
        {

            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
   
    case WM_DESTROY:
        PostQuitMessage(0);
        break;  
    case WM_KEYDOWN:
        ;
        if (wParam == VK_ESCAPE) {
            DestroyWindow(hWnd);
        }
        return 0;
    case WM_LBUTTONDOWN:
        if (Exit.CheckCollisionMouse()) {
            DestroyWindow(hWnd);
        }

    case WM_CHAR: {

        char c = (char)wParam;
        std::stringstream ss;
        ss << "Нажата клавиша: '" << c << "' (Код: " << (int)wParam << ")";
        OutputDebugStringA(ss.str().c_str());
        OutputDebugStringA("\n");
        break;
    }

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
