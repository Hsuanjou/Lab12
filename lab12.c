#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct
{
    double sum;
    int N, size;
    long tid;
    double *a;

} data_;

data_ *createThreadData(double *a, double sum, int N, int size, long tid)
{
    data_ *p = malloc(sizeof(data_));

    p->a = a;

    p->sum = sum;

    p->N = N;

    p->size = size;

    p->tid = tid;

    return p;
}

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
double *a = NULL, sum = 0.0;
int N, size;
void *compute(void *arg)
{
    data_ *arg = (data_ *)argu;
    arg = (data_ *)arg;
    double *ret = NULL;

    int myStart, myEnd, myN, i;
    long tid = (long)arg;
    // determine start and end of computation for the current thread
    myN = N / size;
    myStart = tid * myN;
    myEnd = myStart + myN;
    if (tid == (size - 1))
        myEnd = N;
    // compute partial sum
    double mysum = 0.0;
    for (i = myStart; i < myEnd; i++)
        mysum += a[i];
    // grab the lock, update global sum, and release lock
    pthread_mutex_lock(&mutex);
    sum += mysum;
    pthread_mutex_unlock(&mutex);
    return (NULL);
}
int main(int argc, char **argv)
{
    long i;
    pthread_t *tid;
    if (argc != 3)
    {
        printf("Usage: %s <# of elements> <# of threads>\n", argv[0]);
        exit(-1);
    }

    N = atoi(argv[1]);    // no. of elements
    size = atoi(argv[2]); // no. of threads
    // allocate vector and initialize
    tid = (pthread_t *)malloc(sizeof(pthread_t) * size);
    a = (double *)malloc(sizeof(double) * N);
    for (i = 0; i < N; i++)
        a[i] = (double)(i + 1);
    // create threads
    for (i = 0; i < size; i++)
        pthread_create(&tid[i], NULL, compute, (void *)i);

    // wait for them to complete
    for (i = 0; i < size; i++)
        pthread_join(tid[i], NULL);

    printf("The total is %g, it should be equal to %g\n",
           sum, ((double)N * (N + 1)) / 2);

    return 0;
}