#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include "utilities.h"
#include "queue.h"
#include "reader.h"
#define MAX_CORES 255
#define REALOCATED -1

static void calculate_cpu_usage(cpu_info_t* cpu_statistics, cpu_info_t* prev_cpu_statistics);

extern circ_bbuf_info_t circular_buffer_reader_analyzer;
extern circ_bbuf_info_t circular_buffer_analyzer_printer;

extern pthread_mutex_t mutex_reader_analyzer;
extern pthread_mutex_t mutex_analyzer_printer;

cpu_info_t* prev_cpu_statistics = 0;
int32_t max_cores = 0;

static void calculate_cpu_usage(cpu_info_t* cpu_statistics, cpu_info_t* prev_cpu_statistics) 
{
    unsigned long long int prev_total_cpu = add_elements_in_array(prev_cpu_statistics->cpu_stats.stats_array, (size_t)STATS_ARRAY_SIZE);
    unsigned long long int total_cpu = add_elements_in_array(cpu_statistics->cpu_stats.stats_array, (size_t)STATS_ARRAY_SIZE);

    unsigned long long int difference = total_cpu - prev_total_cpu;
    unsigned long long int difference_without_idle = (total_cpu - cpu_statistics->cpu_stats.idle) - (prev_total_cpu -prev_cpu_statistics->cpu_stats.idle);

    double cpu_usage = (1.0 - ((double)difference_without_idle / difference)) * 100.0;
    cpu_statistics->cpu_usage = cpu_usage;
}

void init_cores_table(void)
{
    prev_cpu_statistics = (cpu_info_t*)calloc(MAX_CORES, sizeof(cpu_info_t));
}

void realoc_cores_table(void)
{
    prev_cpu_statistics = (cpu_info_t*)realloc(prev_cpu_statistics, ((max_cores+1)*sizeof(cpu_info_t)));
}
void free_cores_table(void)
{
    free(prev_cpu_statistics);
}

void read_stats_and_put_result_in_queue(void)
{
    cpu_info_t cpu_statistics = {0};
    pthread_mutex_lock(&mutex_reader_analyzer);
    if (circ_bbuf_pop_info(&circular_buffer_reader_analyzer, &cpu_statistics) == -1) {
        return;
    }
    pthread_mutex_unlock(&mutex_reader_analyzer);
    calculate_cpu_usage(&cpu_statistics, &prev_cpu_statistics[cpu_statistics.cpu_id]);
    if (prev_cpu_statistics[cpu_statistics.cpu_id].cpu_stats.user == 0)
    {
        cpu_statistics.cpu_usage = 0;
    }
    pthread_mutex_lock(&mutex_analyzer_printer);
    if (circ_bbuf_push_info(&circular_buffer_analyzer_printer, cpu_statistics)) {
        return;
    }
    pthread_mutex_unlock(&mutex_analyzer_printer);
    max_cores = ((cpu_statistics.cpu_id > max_cores) && (max_cores != REALOCATED)) ? cpu_statistics.cpu_id : max_cores;
    prev_cpu_statistics[cpu_statistics.cpu_id] = cpu_statistics;
    if((max_cores > cpu_statistics.cpu_id) && (max_cores != REALOCATED))
    {
        realoc_cores_table();
        max_cores = REALOCATED;
    }
}