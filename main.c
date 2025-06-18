#include <stdio.h>
#include "threadpool.h"
//
// Created by justi on 6/18/2025.
//

void red() {
    printf("1");
}
void blue() {
    printf("2");
}
void green() {
    printf("3");
}

int main() {
    // general threadpool, setup the threads and stuff
    TaskFunc tasks_arr[] = { red, green, blue };
    int num_elements = sizeof(tasks_arr) / sizeof(TaskFunc);
    ThreadPool *pool = thread_pool_create(num_elements);

    run(pool, tasks_arr, num_elements);
    return 0;
}