#include <stdio.h>
#include "threadpool.h"
#include "grab_pixel.h"
#include <time.h>
//
// Created by justi on 6/18/2025.

void checkForPixel() {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);  // Start timer

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
            // Optional: insert color logic here
            if (z.rgb.r == 255 && z.rgb.g == 0 && z.rgb.b == 0) {
                printf("Found pure red at (%d, %d)\n", x, y);
            }
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);  // End timer

    double duration_ms = (end.tv_sec - start.tv_sec) * 1000.0 +
                         (end.tv_nsec - start.tv_nsec) / 1e6;

    printf("end with iteration — took %.3f ms\n", duration_ms);
    freeScreenCapture(&sc);
}


//


// create a looker up display thing

int main() {


    // general threadpool, setup the threads and stuff
    TaskFunc tasks_arr[] = { checkForPixel };

    // pass in the size to prevent repetitive use
    int num_elements = sizeof(tasks_arr) / sizeof(TaskFunc);
    // this will setup the pool
    ThreadPool *pool = thread_pool_create(num_elements);

	// now run the tasks in the pool
    run(pool, tasks_arr, num_elements);
    
    return 0;

}