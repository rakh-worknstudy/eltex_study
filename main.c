#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>

#define ICOUNT 100000
pthread_mutex_t mutex;

int test_normal(void);
int test_pthreads(void);
int test_pthreads_safe(void);

void *function_normal(void *args);
void *function_pthreads(void *args);
void *function_pthreads_safe(void *args);

int main(void) {
    test_normal();
    test_pthreads();
    test_pthreads_safe();
    //	printf("%lu\n", PTHREAD_STACK_MIN);
	return 0;
}

int test_normal(void) {
    unsigned long a = 0;
	printf("NORMAL\n");
    time_t t_start = clock();
	printf("  function(): ");
    function_normal((void *)&a);
	printf("DONE\n");
    time_t t_end = clock();
    double t_result = (double)(t_end - t_start) / CLOCKS_PER_SEC;
    printf("  Result: %lu, %lfsec\n", a, t_result);
    return 0;
}
int test_pthreads(void) {
    unsigned long a = 0;
	pthread_t threads[ICOUNT];
	pthread_attr_t attr;
	pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN);
	printf("PTHREAD w/o MUTEX\n");
    time_t t_start = clock();
	printf("  create(): ");
    for (unsigned long iter = 0; iter < ICOUNT; ++iter)
        pthread_create(&threads[iter], &attr, function_pthreads, (void *)&a);
	printf("DONE\n");
	printf("  join(): ");
	fflush(stdout);
	for (unsigned long iter = 0; iter < ICOUNT; ++iter)
		pthread_join(threads[iter], NULL);
	printf("DONE\n");
    time_t t_end = clock();
    double t_result = (double)(t_end - t_start) / CLOCKS_PER_SEC;
    printf("  Result: %lu, %lfsec\n", a, t_result);
    return 0;
}
int test_pthreads_safe(void) {
    unsigned long a = 0;
	pthread_t threads_s[ICOUNT];
	pthread_attr_t attr;
	pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN);
	pthread_mutex_init(&mutex, NULL);
	printf("PTHREAD w MUTEX\n");
    time_t t_start = clock();
	printf("  create(): ");
    for (unsigned int iter = 0; iter < ICOUNT; ++iter)
        pthread_create(&threads_s[iter], &attr, function_pthreads_safe, (void *)&a);
	printf("DONE\n");
	printf("  join(): ");
	for (unsigned int iter = 0; iter < ICOUNT; ++iter)
		pthread_join(threads_s[iter], NULL);
	printf("DONE\n");
    time_t t_end = clock();
    double t_result = (double)(t_end - t_start) / CLOCKS_PER_SEC;
    printf("  Result: %lu, %lfsec\n", a, t_result);
	pthread_mutex_destroy(&mutex);
    return 0;
}

void *function_normal(void *args) {
	for (unsigned int iter = 0; iter < ICOUNT; ++iter)
         ++(*(unsigned int *)(args));
}
void *function_pthreads(void *args) {
	++(*(unsigned int *)(args));
}
void *function_pthreads_safe(void *args) {
	while (pthread_mutex_trylock(&mutex))
		continue;
	++(*(unsigned int *)(args));
	pthread_mutex_unlock(&mutex);
}
