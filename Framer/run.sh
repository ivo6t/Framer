#!/bin/bash

# ---------------- CONFIG ----------------
SRC_DIR="./src"
INCLUDE_DIR="./includes"
LIB_NAME="framer"

# Detect OS
OS=$(uname)
echo "Detected OS: $OS"

# Compiler flags
CXXFLAGS="-I$INCLUDE_DIR -std=c++17"
LDFLAGS=""

# Platform-specific settings
if [[ "$OS" == "Linux" ]]; then
    PLATFORM_SRC="$SRC_DIR/main.cpp"
    LIB_FILE="lib${LIB_NAME}.a"
    # Compile SDL2 and TTF linked statically for example
    LDFLAGS+=" $(sdl2-config --cflags --libs) -lSDL2_ttf"
elif [[ "$OS" == "Darwin" ]]; then
    PLATFORM_SRC="$SRC_DIR/macos.mm"
    LIB_FILE="lib${LIB_NAME}.a"
    CXXFLAGS+=" -ObjC++"
    LDFLAGS+=" -framework Cocoa"
elif [[ "$OS" == "MINGW"* || "$OS" == "MSYS"* || "$OS" == "CYGWIN"* ]]; then
    PLATFORM_SRC="$SRC_DIR/windows_main.cpp"
    LIB_FILE="${LIB_NAME}.lib"
    LDFLAGS+=" -lSDL2 -lSDL2_ttf"
else
    echo "Unsupported OS: $OS"
    exit 1
fi

# Compile framework object
echo "Compiling $PLATFORM_SRC..."
g++ -c $PLATFORM_SRC $CXXFLAGS -o framer.o

if [[ $? -ne 0 ]]; then
    echo "Compilation failed!"
    exit 1
fi

# Create static library
echo "Creating library $LIB_FILE..."
if [[ "$OS" == "Linux" || "$OS" == "Darwin" ]]; then
    ar rcs $LIB_FILE framer.o
elif [[ "$OS" == "MINGW"* || "$OS" == "MSYS"* || "$OS" == "CYGWIN"* ]]; then
    ar rcs $LIB_FILE framer.o
fi

# Add current dir to PATH temporarily
export PATH="$PWD:$PATH"
echo "Added $(pwd) to PATH"

echo "Framework library created successfully: $LIB_FILE"
echo "Users can now compile their program like:"
echo "  g++ my_program.cpp -I$INCLUDE_DIR -L. -lframer $LDFLAGS -o myApp"

