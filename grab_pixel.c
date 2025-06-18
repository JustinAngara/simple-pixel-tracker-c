#include "grab_pixel.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

// Reusable GDI handles
static HDC hdcWindow = NULL;
static HDC hdcMemDC = NULL;
static HBITMAP hBitmap = NULL;
static HGDIOBJ oldObj = NULL;

int initScreenCapture(ScreenCapture* sc) {
    if (!hdcWindow) {
        hdcWindow = GetDC(NULL);
        hdcMemDC = CreateCompatibleDC(hdcWindow);
    }

    // Default to full screen size on first alloc
    sc->width = GetSystemMetrics(SM_CXSCREEN);
    sc->height = GetSystemMetrics(SM_CYSCREEN);

    int w = sc->width;
    int h = sc->height;

    hBitmap = CreateCompatibleBitmap(hdcWindow, w, h);
    oldObj = SelectObject(hdcMemDC, hBitmap);

    int bufferSize = w * h * 4;
    sc->buffer = (BYTE*)malloc(bufferSize);
    return (sc->buffer == NULL) ? -1 : 0;
}

int captureRegion(ScreenCapture* sc, int x1, int y1, int x2, int y2) {
    int width = x2 - x1;
    int height = y2 - y1;

    if (width <= 0 || height <= 0) return -1;

    // Capture the region
    BitBlt(hdcMemDC, 0, 0, width, height, hdcWindow, x1, y1, SRCCOPY | CAPTUREBLT);

    // Setup BMP header
    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    return GetDIBits(hdcMemDC, hBitmap, 0, height, sc->buffer, &bmi, DIB_RGB_COLORS) != 0 ? 0 : -1;
}

color_pixel getPixel(ScreenCapture* sc, int x, int y) {
    RGB color = { -1, -1, -1 };
    int index = (y * sc->width + x) * 4;
    BYTE b = sc->buffer[index];
    BYTE g = sc->buffer[index + 1];
    BYTE r = sc->buffer[index + 2];
    color = (RGB){ r, g, b };
    return (color_pixel){ x, y, color };
}

void freeScreenCapture(ScreenCapture* sc) {
    if (sc->buffer) {
        free(sc->buffer);
        sc->buffer = NULL;
    }
    if (hBitmap) {
        SelectObject(hdcMemDC, oldObj);
        DeleteObject(hBitmap);
        hBitmap = NULL;
    }
    if (hdcMemDC) {
        DeleteDC(hdcMemDC);
        hdcMemDC = NULL;
    }
    if (hdcWindow) {
        ReleaseDC(NULL, hdcWindow);
        hdcWindow = NULL;
    }
}
