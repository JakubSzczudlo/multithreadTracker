#include "pthread.h"
#include "reader.h"

pthread_mutex_t mutex;

int main(int argc, char* argv[])
{
    
    run_reader_thread();
    return 0;


}