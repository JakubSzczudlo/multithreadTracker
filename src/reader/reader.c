#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "reader.h"
#include "queue.h"

static FILE* open_file(const char* file_path);
static void close_file(FILE* fp);
static void skip_first_line_in_file(FILE* fp);

extern circ_bbuf_info_t circular_buffer_reader_analyzer;


static FILE* open_file(const char* file_path)
{
    FILE* fp = fopen(file_path, "r");
    if (fp == 0) 
    {
        perror("Error opening");
        exit(EXIT_FAILURE);
    }
    return fp;
}
static void skip_first_line_in_file(FILE* fp)
{
    int c = 0;
    while ((c = fgetc(fp)) != EOF && c != '\n');
}

static void close_file(FILE* fp)
{
    fclose(fp);
}

void read_cpu_stats(char* line, cpu_info_t* const cpu_statistics)
{
    char* token_end = 0;
    if (strncmp(line, "cpu", 3) == 0) 
    {
        char *token = strtok_r(line, " ", &token_end);

        int cpuNumber;
        sscanf(token, "cpu%d ", &cpuNumber);
        cpu_statistics->cpu_id = (uint16_t)cpuNumber;
        token = strtok_r(0, " ", &token_end); // get rid of "cpu" on beggining

        for(uint8_t i = 0; i < STATS_ARRAY_SIZE; i++)
        {
            cpu_statistics->cpu_stats.stats_array[i] = strtoull(token, (void*)0, 10);
            token = strtok_r(0, " ", &token_end);
        }
    }
}

void put_stats_in_queue(void)
{
    FILE* file = open_file((const char*){"/proc/stat"});
    skip_first_line_in_file(file);
    cpu_info_t cpu_stats = {0};
    char line[256];
    while ((fgets(line, sizeof(line), file) != 0) )
    {
        if((strncmp(line, "cpu", 3) != 0))
        {
            break;
        }
        read_cpu_stats(line, &cpu_stats);
        if (circ_bbuf_push_info(&circular_buffer_reader_analyzer, cpu_stats) == -1) {
            continue;
        }
    }
    close_file(file);
}
