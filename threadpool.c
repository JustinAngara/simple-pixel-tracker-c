//
// Created by justi on 6/17/2025.
//

#include "threadpool.h"
#include "grab_pixel.h"
#include <stdio.h>
#include <unistd.h>
#include "threadpool.h"


static void *thread_worker(void *arg) {
    ThreadPool *pool = (ThreadPool *)arg;
    while (1) {
        pthread_mutex_lock(&pool->lock);
        while (pool->task_queue_head == NULL && !pool->shutdown)
            pthread_cond_wait(&pool->notify, &pool->lock);

        if (pool->shutdown) {
            pthread_mutex_unlock(&pool->lock);
            pthread_exit(NULL);
        }

        Task *task = pool->task_queue_head;
        if (task) {
            pool->task_queue_head = task->next;
        }
        pthread_mutex_unlock(&pool->lock);

        if (task) {
            task->function(task->arg);
            free(task);
        }
    }
    return NULL;
}

ThreadPool *thread_pool_create(int num_threads) {
    ThreadPool *pool = malloc(sizeof(ThreadPool));
    pool->thread_count = num_threads;
    pool->shutdown = 0;
    pool->task_queue_head = NULL;
    pthread_mutex_init(&pool->lock, NULL);
    pthread_cond_init(&pool->notify, NULL);
    pool->threads = malloc(sizeof(pthread_t) * num_threads);

    for (int i = 0; i < num_threads; i++) {
        pthread_create(&pool->threads[i], NULL, thread_worker, pool);
    }
    return pool;
}

void thread_pool_add(ThreadPool *pool, void (*function)(void *), void *arg) {
    Task *new_task = malloc(sizeof(Task));
    new_task->function = function;
    new_task->arg = arg;
    new_task->next = NULL;

    pthread_mutex_lock(&pool->lock);
    Task *head = pool->task_queue_head;
    if (!head) {
        pool->task_queue_head = new_task;
    } else {
        while (head->next) head = head->next;
        head->next = new_task;
    }
    pthread_cond_signal(&pool->notify);
    pthread_mutex_unlock(&pool->lock);
}

void thread_pool_destroy(ThreadPool *pool) {
    pthread_mutex_lock(&pool->lock);
    pool->shutdown = 1;
    pthread_cond_broadcast(&pool->notify);
    pthread_mutex_unlock(&pool->lock);

    for (int i = 0; i < pool->thread_count; i++) {
        pthread_join(pool->threads[i], NULL);
    }

    free(pool->threads);
    pthread_mutex_destroy(&pool->lock);
    pthread_cond_destroy(&pool->notify);
    free(pool);
}

void foo() {
    printf("test1");
}
void foo1() {
    printf("test2");
}

int run(ThreadPool *pool, TaskFunc tasks[], int size) {

    while (1) {

        // iterate to add to new pool
        for (int i = 0; i<size; i++) {
            thread_pool_add(pool, tasks[i], NULL);
        }

    }
    thread_pool_destroy(pool);

    return 0;
}