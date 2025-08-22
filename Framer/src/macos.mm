#import <Cocoa/Cocoa.h>
#include "../includes/framer.h"

NSWindow* gWindow = nil;
NSTextField* gTextField = nil;
bool gRunning = true;

void init() {
    // nothing extra needed
}

void createWindow(const std::string& title, int width, int height, SDL_Window** window) {
    @autoreleasepool {
        [NSApplication sharedApplication];

        NSUInteger style = NSWindowStyleMaskTitled |
                           NSWindowStyleMaskClosable |
                           NSWindowStyleMaskResizable;

        gWindow = [[NSWindow alloc] initWithContentRect:NSMakeRect(0,0,width,height)
                                               styleMask:style
                                                 backing:NSBackingStoreBuffered
                                                   defer:NO];

        [gWindow setTitle:[NSString stringWithUTF8String:title.c_str()]];
        [gWindow makeKeyAndOrderFront:nil];

        // Setup TextField for TextBox demo
        gTextField = [[NSTextField alloc] initWithFrame:NSMakeRect(100, 200, 440, 40)];
        [[gWindow contentView] addSubview:gTextField];
    }
}

void x_eleven(bool forceX11) {
    // no-op on macOS
}

// ---------------- TextBox -------------------
TextBox::TextBox(int x_, int y_, int w_, int h_) : x(x_), y(y_), w(w_), h(h_), text("") {}

TextBox::~TextBox() {}

void TextBox::handleEvent(SDL_Event& e) {
    // macOS events not mapped here, use NSTextField directly
    text = [[gTextField stringValue] UTF8String];
}

void TextBox::draw() {
    @autoreleasepool {
        [gTextField setStringValue:[NSString stringWithUTF8String:text.c_str()]];

        // Draw box background
        [gTextField setBackgroundColor:[NSColor colorWithCalibratedRed:0.78 green:0.78 blue:0.78 alpha:1.0]];
        [gTextField setBezeled:YES];
        [gTextField setEditable:YES];
    }
}

std::string TextBox::getText() { return text; }
