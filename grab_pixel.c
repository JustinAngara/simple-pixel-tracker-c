#include "grab_pixel.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * call this at the start
 * This will allocate the memory, bitblt, bitmap, etc to grab screen content
 */
int initScreenCapture(ScreenCapture* sc) {
    HDC hdcScreen = GetDC(NULL);
    if (!hdcScreen) {
        printf("Error: Failed to get screen DC.\n");
        return -1;
    }

    int w = GetDeviceCaps(hdcScreen, HORZRES);
    int h = GetDeviceCaps(hdcScreen, VERTRES);
    int bufferSize = w * h * 4;

    // Allocate or reallocate buffer if needed
    if (sc->buffer == NULL || sc->width != w || sc->height != h) {
        if (sc->buffer != NULL) free(sc->buffer);
        sc->buffer = (BYTE*)malloc(bufferSize);
        if (!sc->buffer) {
            printf("Error: Failed to allocate buffer.\n");
            ReleaseDC(NULL, hdcScreen);
            return -1;
        }
    }

    // Create memory DC and bitmap
    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, w, h);
    if (!hBitmap) {
        printf("Error: Failed to create bitmap.\n");
        ReleaseDC(NULL, hdcScreen);
        return -1;
    }

    SelectObject(hdcMem, hBitmap);
    BitBlt(hdcMem, 0, 0, w, h, hdcScreen, 0, 0, SRCCOPY);

    // Bitmap info setup
    BITMAPINFO bmi;
    ZeroMemory(&bmi, sizeof(BITMAPINFO));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = w;
    bmi.bmiHeader.biHeight = -h; // top-down
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    if (!GetDIBits(hdcMem, hBitmap, 0, h, sc->buffer, &bmi, DIB_RGB_COLORS)) {
        printf("Error: Failed to get DIBits.\n");
        free(sc->buffer);
        sc->buffer = NULL;
        DeleteObject(hBitmap);
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);
        return -1;
    }

    sc->width = w;
    sc->height = h;

    // Cleanup
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdcScreen);
    return 0;
}


/*
 * once init is called, you are able to have a pointer to memory of the pixel info
 * Calling withut sc being initalized won't work
 */
color_pixel getPixel(ScreenCapture* sc, int x, int y) {
    RGB color = { -1, -1, -1 };

    if (x >= 0 && x < sc->width && y >= 0 && y < sc->height && sc->buffer) {
        int index = (y * sc->width + x) * 4;
        BYTE b = sc->buffer[index];
        BYTE g = sc->buffer[index + 1];
        BYTE r = sc->buffer[index + 2];
        color = (RGB){ r, g, b };
    }

    return (color_pixel){ x, y, color };
}


/*
 *will release the memory, not being in used
 */
void freeScreenCapture(ScreenCapture* sc) {
    if (sc->buffer != NULL) {
        free(sc->buffer);
        sc->buffer = NULL;
    }
    sc->width = 0;
    sc->height = 0;
}
