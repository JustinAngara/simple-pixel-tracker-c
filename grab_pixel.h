#ifndef GRAB_PIXEL_H
#define GRAB_PIXEL_H

#include <windows.h>

// Struct to represent an RGB color
typedef struct {
    int r;
    int g;
    int b;
} RGB;

// Struct to represent a color pixel (with position + color)
typedef struct {
    int x;
    int y;
    RGB rgb;
} color_pixel;

// Struct to represent a screen capture context
typedef struct {
    BYTE* buffer;
    int width;
    int height;
} ScreenCapture;

// Initializes screen capture (allocates and captures the screen)
int initScreenCapture(ScreenCapture* sc);

// Gets a color pixel from the capture buffer
color_pixel getPixel(ScreenCapture* sc, int x, int y);

// Frees the memory held by the capture buffer
void freeScreenCapture(ScreenCapture* sc);

#endif // GRAB_PIXEL_H
