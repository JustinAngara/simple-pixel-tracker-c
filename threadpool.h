#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>

// Task structure
typedef struct Task {
    void (*function)(void *);
    void *arg;
    struct Task *next;
} Task;

// Thread pool structure
typedef struct ThreadPool {
    pthread_mutex_t lock;
    pthread_cond_t notify;
    pthread_t *threads;
    Task *task_queue_head;
    int thread_count;
    int shutdown;
} ThreadPool;

// Function declarations
ThreadPool *thread_pool_create(int num_threads);
void thread_pool_add(ThreadPool *pool, void (*function)(void *), void *arg);
void thread_pool_destroy(ThreadPool *pool);\
typedef void (*TaskFunc)(void *);
int run(ThreadPool *pool, TaskFunc tasks[], int size);
#endif // THREADPOOL_H
