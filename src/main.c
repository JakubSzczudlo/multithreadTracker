#include "pthread.h"
#include "reader.h"

pthread_mutex_t mutex;

int main()
{
    run_reader_thread();
    return 0;
}