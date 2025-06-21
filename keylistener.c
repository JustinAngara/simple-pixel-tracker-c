//
// Created by justi on 6/17/2025.
//

#include <stdio.h>
#include <windows.h>
#include "keylistener.h"

void listenClick() {
    printf("Key listener started...\n");
    while (1) {
        // Check for space key press
        if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
            printf("Space key pressed!\n");
            // Small delay to avoid spam
            Sleep(200);
        }
        // Small delay to prevent 100% CPU usage
        Sleep(10);
    }
}