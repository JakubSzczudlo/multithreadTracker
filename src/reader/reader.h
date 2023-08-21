#ifndef READER_H
#define READER_H
#include <stdint.h>
#define STATS_ARRAY_SIZE 11

typedef union
{
    struct
    {
    uint64_t user;
    uint64_t nice;
    uint64_t system;
    uint64_t idle;
    uint64_t iowait;
    uint64_t irq;
    uint64_t softirq;
    uint64_t steal;
    uint64_t guest;
    uint64_t guest_nice;
    };
    uint64_t stats_array[STATS_ARRAY_SIZE];
}cpu_stats_t; 

typedef struct
{
    uint16_t cpu_id; 
    double cpu_usage;
    cpu_stats_t cpu_stats;
}cpu_info_t;

void run_reader_thread(void);

#endif // READER_H