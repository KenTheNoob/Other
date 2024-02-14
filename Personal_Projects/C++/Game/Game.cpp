//Display a image that you can move with wasd
//g++ Game.cpp -o Game.exe -lgdi32 -lgdiplus
#include <Windows.h>
#include <gdiplus.h>
#include <cmath>

#include <limits.h>
#include <unistd.h>
#include <string.h>

//remove later
#include <iostream>

using namespace std;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int winX = 1280;
int winY = 720;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    //Initialize GDI+
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
    //Register the window class
    const char *CLASS_NAME = "My Window Class";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    //Create the window
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles
        CLASS_NAME,                     // Window class
        "My Window",                    // Window text
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,            // Window style
        //Position and size
        0, 0, winX, winY,
        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );
    //60fps timer forces redraw
    SetTimer(hwnd, 1, 17, NULL);
    //Window loop runs until program exit
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    //Shutdown GDI+
    Gdiplus::GdiplusShutdown(gdiplusToken);
    return 0;
}

double imageX = 640;
double imageY = 360;
double imageX_vel = 0;
double imageY_vel = 0;
HBITMAP hBufferBitmap = NULL;
HDC hBufferDC = NULL;
int bufferWidth, bufferHeight;
bool leftPressed = false;
bool rightPressed = false;
bool upPressed = false;
bool downPressed = false;
double resizeX = 1;
double resizeY = 1;
char cwd[PATH_MAX];
wchar_t path[PATH_MAX];

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg){
    //Set up the double buffer to prevent flicker
    case WM_CREATE:{
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);
        bufferWidth = clientRect.right - clientRect.left;
        bufferHeight = clientRect.bottom - clientRect.top;
        HDC hdc = GetDC(hwnd);
        hBufferDC = CreateCompatibleDC(hdc);
        hBufferBitmap = CreateCompatibleBitmap(hdc, bufferWidth, bufferHeight);
        SelectObject(hBufferDC, hBufferBitmap);
        ReleaseDC(hwnd, hdc);

        //Get image path and convert to wchar_t
        getcwd(cwd, sizeof(cwd));
        strcat(cwd, "\\Images\\P1.png");
        mbstowcs(&path[0], cwd, strlen(cwd)+1);
    }
    break;
    case WM_DESTROY:{
        if (hBufferDC)
        {
            DeleteDC(hBufferDC);
            hBufferDC = NULL;
        }
        if (hBufferBitmap)
        {
            DeleteObject(hBufferBitmap);
            hBufferBitmap = NULL;
        }
        PostQuitMessage(0);
    }
        break;
    case WM_SIZE:{
        bufferWidth = LOWORD(lParam);
        bufferHeight = HIWORD(lParam);
        resizeX = double(bufferWidth)/double(winX);
        resizeY = double(bufferHeight)/double(winY);
        //cout << bufferWidth << " " << resizeX << " " << bufferHeight << " " << resizeY << endl;
        winX = bufferWidth;
        winY = bufferHeight;
        imageX *= resizeX;
        imageY *= resizeY;
        // Resize the buffer and bitmap
        if (hBufferBitmap)
        {
            DeleteObject(hBufferBitmap);
            hBufferBitmap = CreateCompatibleBitmap(GetDC(hwnd), bufferWidth, bufferHeight);
            SelectObject(hBufferDC, hBufferBitmap);
        }

        RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
    }
        break;
    case WM_PAINT:{
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        if(hBufferDC){
            //Fill in white background
            HBRUSH hBackgroundBrush = CreateSolidBrush(RGB(255, 255, 255)); // White color
            FillRect(hBufferDC, &ps.rcPaint, hBackgroundBrush);
            DeleteObject(hBackgroundBrush);
            //Draw image
            Gdiplus::Graphics graphics(hBufferDC);
            Gdiplus::Bitmap bmp(path);
            graphics.DrawImage(&bmp, imageX, imageY, winX/20, winY/20);
            BitBlt(hdc, 0, 0, bufferWidth, bufferHeight, hBufferDC, 0, 0, SRCCOPY);
        }
        EndPaint(hwnd, &ps);
    }
        break;
    case WM_KEYDOWN:
        switch(wParam){
            case VK_LEFT: case 'A':
                leftPressed = true;
                break;
            case VK_RIGHT: case 'D':
                rightPressed = true;
                break;
            case VK_UP: case 'W':
                upPressed = true;
                break;
            case VK_DOWN: case 'S':
                downPressed = true;
                break;
        }
        break;
    case WM_KEYUP:
        switch(wParam){
            case VK_LEFT: case 'A':
                leftPressed = false;
                break;
            case VK_RIGHT: case 'D':
                rightPressed = false;
                break;
            case VK_UP: case 'W':
                upPressed = false;
                break;
            case VK_DOWN: case 'S':
                downPressed = false;
                break;
        }
        break;
    case WM_TIMER:{
        imageX_vel = 0;
        imageY_vel = 0;
        if(leftPressed) imageX_vel -= winX/50;
        if(rightPressed) imageX_vel += winX/50;
        if(upPressed) imageY_vel -= winY/50;
        if(downPressed) imageY_vel += winY/50;
        if(fabs(imageX_vel) + fabs(imageY_vel) >= 20){
            imageX_vel *= 0.7;
            imageY_vel *= 0.7;
        }
        imageX += imageX_vel;
        imageY += imageY_vel;
        RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
    }
    break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}