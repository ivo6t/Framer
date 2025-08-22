#ifndef FRAMER_H
#define FRAMER_H

#include <string>
#include <iostream>
using namespace std;

// ------------------ Platform Includes ------------------
#ifdef _WIN32
#include <windows.h>
#elif __APPLE__
// Objective-C++ will include Cocoa in macos_main.mm
#elif __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#endif

// ------------------ Global Window/Renderer ------------------
#ifdef _WIN32
extern HWND gWindow;
extern HDC gDC;
extern HFONT gFont;
#elif __APPLE__
// macOS globals are in macos_main.mm
#elif __linux__
extern SDL_Renderer* gRenderer;
extern SDL_Window* gWindow;
extern TTF_Font* gFont;
#endif

// ------------------ API Functions ------------------

// Initialize platform + font
void init();

// Create a window
void createWindow(const std::string& title, int width, int height, 
#ifdef __linux__
                 SDL_Window** window = nullptr
#else
                 void* window = nullptr // placeholder for Win/macOS
#endif
);

// Force X11 or Wayland (Linux only)
void x_eleven(bool forceX11);

// -------------------- TextBox --------------------
class TextBox {
public:
    TextBox(int x, int y, int w, int h);
    ~TextBox();

    // Unified event API
#ifdef __linux__
    void handleEvent(SDL_Event& e);
#else
    void handleEvent(void* e); // platform-specific events
#endif

    void draw();
    string getText();

private:
    int x, y, w, h;
    string text;

#ifdef _WIN32
    HWND window;
    HDC renderer;
    HFONT font;
#elif __APPLE__
    void* window;   // NSWindow*
    void* renderer; // NSTextField* or similar
    void* font;     // NSFont* or nil
#elif __linux__
    SDL_Renderer* renderer;
    SDL_Window* window;
    TTF_Font* font;
#endif
};

#endif // FRAMER_H
