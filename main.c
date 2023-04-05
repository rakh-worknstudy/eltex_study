#include <pthread.h>
#include <stdio.h>
#include <time.h>

#define ICOUNT 20000
pthread_mutex_t mutex;

int test_normal(void);
int test_pthreads(void);
int test_pthreads_safe(void);

void *function_pthreads(void *args);
void *function_pthreads_safe(void *args);

int main(void) {
    test_normal();
    test_pthreads();
    test_pthreads_safe();
    return 0;
}

int test_normal(void) {
    unsigned int a = 0;
    time_t t_start = clock();
    for (unsigned int iter = 0; iter < ICOUNT; ++iter)
        ++a;
    time_t t_end = clock();
    double t_result = (double)(t_end - t_start) / CLOCKS_PER_SEC;
    printf("Normal: %d, %lfsec\n", a, t_result);
    return 0;
}
int test_pthreads(void) {
    unsigned int a = 0;
    pthread_t threads[ICOUNT];
    time_t t_start = clock();
    for (unsigned int iter = 0; iter < ICOUNT; ++iter)
        pthread_create(&threads[iter], NULL, function_pthreads, &a);
    time_t t_end = clock();
    double t_result = (double)(t_end - t_start) / CLOCKS_PER_SEC;
    printf("Threads: %d, %lfsec\n", a, t_result);
    return 0;
}
int test_pthreads_safe(void) {
    unsigned int a = 0;
    pthread_t threads_s[ICOUNT];
    pthread_mutex_init(&mutex, NULL);
    time_t t_start = clock();
    for (unsigned int iter = 0; iter < ICOUNT; ++iter)
        pthread_create(&threads_s[iter], NULL, function_pthreads_safe, &a);
    time_t t_end = clock();
    pthread_mutex_destroy(&mutex);
    double t_result = (double)(t_end - t_start) / CLOCKS_PER_SEC;
    printf("Threads w/ Mutex: %d, %lfsec\n", a, t_result);
    return 0;
}

void *function_pthreads(void *args) {
    ++(*(unsigned int *)(args));
    return 0;
}
void *function_pthreads_safe(void *args) {
    pthread_mutex_lock(&mutex);
    ++(*(unsigned int *)(args));
    pthread_mutex_unlock(&mutex);
    pthread_exit(0);
}