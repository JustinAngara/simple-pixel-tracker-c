//
// Created by justi on 6/17/2025.
//

#include "main.h"
#include "grab_pixel.h"
#include <stdio.h>



int main(){
    // this is used to point at the screen
    ScreenCapture capture = { 0 };

    // initalization didn't occur correctly
    if (initScreenCapture(&capture) != 0) {
        return 1;
    }

    // now you can grab the pixel loc since it is inits
    color_pixel pixel = getPixel(&capture, 100, 100);
    printf("Pixel at (%d, %d): R=%d, G=%d, B=%d\n",
           pixel.x, pixel.y,
           pixel.rgb.r, pixel.rgb.g, pixel.rgb.b);


    // no longer being used
    freeScreenCapture(&capture);
    return 0;
}
