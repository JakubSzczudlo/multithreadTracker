#include "pthread.h"
#include "semaphore.h"

sem_t produced_reader;
sem_t produced_analyzer;
pthread_mutex_t mutex_reader_analyzer;
pthread_mutex_t mutex_analyzer_printer;

int main()
{
    run_reader_thread();
    return 0;
}