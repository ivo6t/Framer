#include "../includes/framer.h"
#include <windows.h>

HWND gWindow = nullptr;
HDC gDC = nullptr;
HFONT gFont = nullptr;
bool gRunning = true;

// Simple event mapping
struct Event {
    enum Type { Quit, KeyDown, TextInput } type;
    char text[32];
    int key;
};
Event gEvent;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CLOSE:
            gRunning = false;
            PostQuitMessage(0);
            return 0;

        case WM_CHAR:
            gEvent.type = Event::TextInput;
            gEvent.text[0] = (char)wParam;
            gEvent.text[1] = 0;
            return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void init() {
    // nothing for Win32
}

void createWindow(const std::string& title, int width, int height, SDL_Window** window) {
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "FramerWin";

    RegisterClass(&wc);

    gWindow = CreateWindowEx(0, wc.lpszClassName, title.c_str(),
                             WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                             CW_USEDEFAULT, CW_USEDEFAULT, width, height,
                             NULL, NULL, wc.hInstance, NULL);

    gDC = GetDC(gWindow);
    gFont = CreateFont(24,0,0,0,FW_NORMAL, FALSE,FALSE,FALSE,
                       DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
                       CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
                       VARIABLE_PITCH, TEXT("Arial"));
}

void x_eleven(bool forceX11) {
    // no-op on Windows
}

// ---------------- TextBox -------------------
TextBox::TextBox(int x_, int y_, int w_, int h_) : x(x_), y(y_), w(w_), h(h_), text("") {}

TextBox::~TextBox() {}

void TextBox::handleEvent(SDL_Event& e) {
    // Not SDL on Windows; you need a custom mapping
    if (gEvent.type == Event::TextInput)
        text += gEvent.text;
}

void TextBox::draw() {
    if (!gDC || !gFont) return;

    // Draw box
    RECT rect = { x, y, x + w, y + h };
    HBRUSH brush = CreateSolidBrush(RGB(200,200,200));
    FillRect(gDC, &rect, brush);
    DeleteObject(brush);

    // Draw text
    SelectObject(gDC, gFont);
    SetTextColor(gDC, RGB(0,0,0));
    SetBkMode(gDC, TRANSPARENT);
    DrawTextA(gDC, text.c_str(), text.size(), &rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
}

std::string TextBox::getText() { return text; }
