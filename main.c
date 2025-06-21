#include <stdio.h>
#include "threadpool.h"
#include "grab_pixel.h"
#include "keylistener.h"
//
// Created by justi on 6/18/2025.

void checkForPixel() {
    // Setup capture context once
    ScreenCapture sc = {0};
    if (initScreenCapture(&sc) != 0) {
        printf("Initialization failed.\n");
        return;
    }

    // Capture only 1920x1080 region from top-left
    int x1 = 0, y1 = 0, x2 = 1920, y2 = 1080;
    if (captureRegion(&sc, x1, y1, x2, y2) != 0) {
        printf("Capture failed.\n");
        freeScreenCapture(&sc);
        return;
    }

    // Process pixel buffer
    for (int y = 0; y < (y2 - y1); y++) {
        for (int x = 0; x < (x2 - x1); x++) {
            color_pixel z = getPixel(&sc, x, y);
            if (z.rgb.r == 255 && z.rgb.g == 0 && z.rgb.b == 0) {
                printf("Found pure red at (%d, %d)\n", x, y);
            }
        }
    }

    freeScreenCapture(&sc);
}

// create a looker up display thing

int main() {
    // Test checkForPixel first to make sure it works
    printf("Testing pixel capture...\n");
    checkForPixel();

    // If that worked, try with thread pool but only 1 thread
    TaskFunc tasks_arr[] = { checkForPixel, listenClick };
    int num_elements = sizeof(tasks_arr) / sizeof(TaskFunc);

    // Create pool with only 1 thread to avoid conflicts
    ThreadPool *pool = thread_pool_create(1);  // Force single thread

    run(pool, tasks_arr, num_elements);

    return 0;
}