#include <pthread.h>


static void* reader_thread(void* pArg)
{
    (void) pArg;

    return 0;
}


void run_reader_thread(void)
{
    pthread_t thread_id = 0U;
    pthread_attr_t threadAttr;

    pthread_attr_init(&threadAttr);
    pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);

    pthread_create(&thread_id, &threadAttr, reader_thread, 0);

    pthread_attr_destroy(&threadAttr);
}