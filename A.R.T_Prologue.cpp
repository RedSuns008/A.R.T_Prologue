//------------------------------------------------------------------------------//
#pragma comment(lib, "Msimg32.lib")
#include "framework.h"
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
 
//------------------------------------------------------------------------------//


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
//------------------------------------------------------------------------------//

void ShowInventory() {
    //if (Inventory.CheckCollisionMouse()) {
        //ShowBitmap(Inventory.x, Inventory.y, Inventory.width, Inventory.height, Inventory.hBitmap, true);
    ShowBitmap(1500, 1000, 100, 100, Inventory.hBitmap, true);

    //}
}




int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int  nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    HDC hdcScreen = GetDC(window.hWnd);
    window.context = CreateCompatibleDC(hdcScreen);
    window.width = GetSystemMetrics(SM_CXSCREEN);
    window.height = GetSystemMetrics(SM_CYSCREEN);
    player.speed = 10;
    player.x = window.width / 2.;//ракетка посередине окна
    player.y = window.height/ 2;
    


    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ARTPROLOGUE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    Exit.Load("exit_butt.bmp", "exit_butt_glow.bmp", 100, 100, 150, 50);
    player.Load("exit_butt.bmp", player.x, player.y, 50,50, 10);
    Inventory.Load("exit_butt.bmp", "exit_butt_glow.bmp", 1500, 1000, 100, 100);
    BackGround_bmp = (HBITMAP)LoadImageA(NULL, "phon1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ARTPROLOGUE));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {

        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            Mouse.Update();
            player.prevX = player.x;
            player.prevY = player.y;

            player.ProcessInput();

            // Если игрок движется, перерисовываем только измененные области
            if (player.isMoving) {
                RECT prevRect = player.GetPrevRect();
                RECT currentRect = player.GetRect();

                InvalidateRect(window.hWnd, &prevRect, FALSE);
                InvalidateRect(window.hWnd, &currentRect, FALSE);
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            
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
    HDC hdcMem = CreateCompatibleDC(window.context);
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
    case WM_RBUTTONDOWN:
        Exit.StartDragging();
        if (Exit.isDragging) {
            SetCapture(hWnd); // Захватываем мышь
            
            lastButtonRect = {(LONG)Exit.x,(LONG)Exit.y,(LONG)(Exit.x + Exit.width),(LONG)(Exit.y + Exit.height)}; // Запоминаем текущую позицию кнопки
        }
        break;

    case WM_MOUSEMOVE:
    {
       



        // Обновляем позицию мыши
        POINT pt = { LOWORD(lParam), HIWORD(lParam) };
        Mouse.x = (float)pt.x;
        Mouse.y = (float)pt.y;

      

        bool wasHovered = Exit.isHovered;    // Обновляем состояние наведения
        Exit.isHovered = Exit.CheckCollisionMouse();

        
        if (Exit.isDragging) { // Обновляем перетаскивание если активно
         
            InvalidateRect(hWnd, &lastButtonRect, FALSE);    // Сначала инвалидируем старую область

            Exit.UpdateDragging();

            
            lastButtonRect = {(LONG)Exit.x,(LONG)Exit.y,(LONG)(Exit.x + Exit.width),(LONG)(Exit.y + Exit.height)};// Обновляем область для перерисовки

            // Инвалидируем новую область
            InvalidateRect(hWnd, &lastButtonRect, FALSE);
        }
        
        else if (wasHovered != Exit.isHovered) {     // Если состояние наведения изменилось, перерисовываем кнопку
            RECT buttonRect = {(LONG)Exit.x,(LONG)Exit.y,(LONG)(Exit.x + Exit.width),(LONG)(Exit.y + Exit.height)};
            InvalidateRect(hWnd, &buttonRect, FALSE);
        }
        break;
    }

    case WM_RBUTTONUP:
        if (Exit.isDragging) {
            Exit.isDragging = false;
            ReleaseCapture(); // Освобождаем захват мыши

            // Перерисовываем область кнопки
            RECT buttonRect = {(LONG)Exit.x,(LONG)Exit.y,(LONG)(Exit.x + Exit.width),(LONG)(Exit.y + Exit.height)};

            InvalidateRect(hWnd, &buttonRect, FALSE);
        }
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        window.context = BeginPaint(hWnd, &ps);

        // Отрисовываем только необходимые области
        for (int i = 0; i < ps.rcPaint.bottom; i++) {
            // Если область перерисовки включает фон
            if (ps.rcPaint.top <= i && i < ps.rcPaint.bottom) {
                DrawBackground(window.context, window.width, window.height, BackGround_bmp);
                break;
            }
        }

        // Отрисовываем кнопку, если она в области перерисовки
        RECT buttonRect = {(LONG)Exit.x,(LONG)Exit.y,(LONG)(Exit.x + Exit.width),(LONG)(Exit.y + Exit.height)};

        //Exit.Show(window.context);
       
        Exit.Show(window.context, false);
        player.Show();
        ShowInventory();
       
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_TIMER:
        SetTimer(hWnd, 1, 16, NULL);
        player.ProcessInput();
        if (player.isMoving) {
            RECT prevRect = player.GetPrevRect();
            RECT currentRect = player.GetRect();
            InvalidateRect(hWnd, &prevRect, FALSE);
            InvalidateRect(hWnd, &currentRect, FALSE);
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
