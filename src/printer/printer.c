#include <stdio.h>
#include "queue.h"
#include <stdlib.h>

extern circ_bbuf_info_t circular_buffer_analyzer_printer;

void print_usage(){
    cpu_info_t cpu_statistics = {0};
    if (circ_bbuf_pop_info(&circular_buffer_analyzer_printer, &cpu_statistics)) {
        exit(EXIT_FAILURE);
    }
    double cpu_usage = cpu_statistics.cpu_usage;
    printf("CPU core: %d, usage: %.2f%%\n", (int)cpu_statistics.cpu_id, cpu_usage);
    }