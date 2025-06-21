#ifndef GRAB_PIXEL_H
#define GRAB_PIXEL_H

#include <windows.h>

// struct to represent an rgb color
typedef struct {
    int r;
    int g;
    int b;
} RGB;

// struct to represent a color pixel (with position + color)
typedef struct {
    int x;
    int y;
    RGB rgb;
} color_pixel;

// struct to represent a screen capture context
typedef struct {
    BYTE* buffer;    // pointer to raw pixel buffer
    int width;       // width of captured region
    int height;      // height of captured region
} ScreenCapture;

/*
 * initializes reusable gdi handles and allocates initial buffer.
 * must be called before captureRegion()
 * returns 0 on success, -1 on failure
 */
int initScreenCapture(ScreenCapture* sc);

/*
 * captures a rectangular screen region (x1, y1) to (x2, y2)
 * into the internal buffer. region size must be consistent with
 * what was allocated in initScreenCapture
 * returns 0 on success, -1 on failure
 */
int captureRegion(ScreenCapture* sc, int x1, int y1, int x2, int y2);

/*
 * retrieves the color of the pixel at (x, y) from the capture buffer
 * coordinates are relative to the captured region, not screen coordinates
 */
color_pixel getPixel(ScreenCapture* sc, int x, int y);

/*
 * frees internal buffer and cleans up gdi resources
 */
void freeScreenCapture(ScreenCapture* sc);

#endif // GRAB_PIXEL_H
