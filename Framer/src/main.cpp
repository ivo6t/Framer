#include "../includes/framer.h"
#include <cstdlib>

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
TTF_Font* gFont = nullptr;

void init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        cout << "SDL_Init Error: " << SDL_GetError() << endl;
        exit(1);
    }

    if (TTF_Init() == -1) {
        cout << "TTF_Init Error: " << TTF_GetError() << endl;
        exit(1);
    }

    gFont = TTF_OpenFont("/usr/share/fonts/dejavu-sans-fonts/DejaVuSans.ttf", 24);
    if (!gFont) {
        cout << "Failed to load font: " << TTF_GetError() << endl;
        exit(1);
    }
}

void createWindow(const string& title, int width, int height, SDL_Window** window) {
    gWindow = SDL_CreateWindow(title.c_str(),
                               SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               width, height,
                               SDL_WINDOW_SHOWN);

    if (!gWindow) {
        cout << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
        SDL_Quit();
        exit(1);
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (!gRenderer) {
        cout << "SDL_CreateRenderer Error: " << SDL_GetError() << endl;
        SDL_DestroyWindow(gWindow);
        SDL_Quit();
        exit(1);
    }

    *window = gWindow;
}


void x_eleven(bool forceX11) {
    if (forceX11) setenv("SDL_VIDEODRIVER", "x11", 1);
    else setenv("SDL_VIDEODRIVER", "wayland", 1);
}

// ---------------- TextBox -------------------
TextBox::TextBox(int x, int y, int w, int h) : x(x), y(y), w(w), h(h), text("") {
    renderer = gRenderer;
    window = gWindow;
    font = gFont;
    SDL_StartTextInput();
}

TextBox::~TextBox() {
    SDL_StopTextInput();
}

void TextBox::handleEvent(SDL_Event& e) {
    if (e.type == SDL_TEXTINPUT) text += e.text.text;
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_BACKSPACE && text.length() > 0)
        text.pop_back();
}

void TextBox::draw() {
    if (!renderer || !font) return;  // safety check

    // Draw box
    SDL_Rect boxRect = { x, y, w, h };
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &boxRect);

    // Draw text
    SDL_Color color = {0,0,0,255};
    SDL_Surface* surf = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!surf) {
        cout << "TTF_RenderText_Solid failed: " << TTF_GetError() << endl;
        return;
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    if (!tex) {
        cout << "SDL_CreateTextureFromSurface failed: " << SDL_GetError() << endl;
        SDL_FreeSurface(surf);
        return;
    }

    SDL_Rect dst = { x+5, y+5, surf->w, surf->h };
    SDL_RenderCopy(renderer, tex, NULL, &dst);

    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
}


string TextBox::getText() { return text; }
