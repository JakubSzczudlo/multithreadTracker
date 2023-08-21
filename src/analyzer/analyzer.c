#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "queue.h"
#include "reader.h"

extern circ_bbuf_info_t circular_buffer_reader_analyzer;
extern circ_bbuf_info_t circular_buffer_analyzer_printer;

static cpu_info_t prev_cpu_statistics = {0};
static unsigned long long int prev_total_cpu = 0;

unsigned long long int add_elements_in_array(uint64_t array[], size_t size)
{
    unsigned long long int sum = 0;
    for(uint8_t i = 0; i < size; i++)
    {
        sum += array[i];
    }
    return sum;
}

void calculate_cpu_usage(cpu_info_t* cpu_statistics) 
{
    unsigned long long int total_cpu = add_elements_in_array(cpu_statistics->cpu_stats.stats_array, (size_t)STATS_ARRAY_SIZE);

    unsigned long long int difference = total_cpu - prev_total_cpu;
    unsigned long long int difference_without_idle = (total_cpu - cpu_statistics->cpu_stats.idle) - (prev_total_cpu -prev_cpu_statistics.cpu_stats.idle);

    double cpu_usage = (1.0 - ((double)difference_without_idle / difference)) * 100.0;
    cpu_statistics->cpu_usage = cpu_usage;
    
    prev_total_cpu = total_cpu;
}

void read_stats_and_put_result_in_queue(void)
{
    cpu_info_t cpu_statistics = {0};
    if (circ_bbuf_pop_info(&circular_buffer_analyzer_printer, &cpu_statistics)) {
        exit(EXIT_FAILURE);
    }
    calculate_cpu_usage(&cpu_statistics);
    if (prev_cpu_statistics.cpu_stats.guest == 0)
    {
        cpu_statistics.cpu_usage = 0;
    }
    if (circ_bbuf_push_info(&circular_buffer_analyzer_printer, cpu_statistics)) {
        exit(EXIT_FAILURE);
    }
    
    prev_cpu_statistics = cpu_statistics;
}