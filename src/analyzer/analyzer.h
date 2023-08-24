#ifndef ANALYZER_H
#define ANALYZER_H

#include <stdint.h>
#include <stdlib.h>
#include "reader.h"

typedef struct
{
    double cpu_usage;
    uint8_t cpu_id;
}cpu_usage_t;

void run_analyzer_thread(void);
unsigned long long int add_elements_in_array(uint64_t array[], size_t size);
void read_stats_and_put_result_in_queue(void);
void init_cores_table(void);
void free_cores_table(void);

#endif // ANALYZER_H