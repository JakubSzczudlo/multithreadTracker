#include <stdio.h>
#include "pthread.h"
#include "semaphore.h"
#include "reader.h"
#include <unistd.h>
#include "analyzer.h"
#include "printer.h"

sem_t produced_reader;
sem_t produced_analyzer;
pthread_mutex_t mutex_reader_analyzer = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_analyzer_printer = PTHREAD_MUTEX_INITIALIZER;

int main()
{
    run_reader_thread();
    run_analyzer_thread();
    run_printer_thread();
    usleep(100000000);
    return 0;
}