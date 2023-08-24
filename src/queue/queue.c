#include <stdio.h>
#include <stdlib.h>
#include "analyzer.h"
#include "queue.h"
#define MAX_BUFFER_SIZE 100

static cpu_info_t queue_stats_analyzer[MAX_BUFFER_SIZE];
static cpu_info_t queue_stats_printer[MAX_BUFFER_SIZE];

circ_bbuf_info_t circular_buffer_reader_analyzer = {queue_stats_analyzer, (int)0, (int)0, (int)MAX_BUFFER_SIZE};
circ_bbuf_info_t circular_buffer_analyzer_printer = {queue_stats_printer, (int)0, (int)0, (int)MAX_BUFFER_SIZE};

circ_bbuf_info_t get_queue_reader_analyzer(void){
    return circular_buffer_reader_analyzer;
}

circ_bbuf_info_t get_queue_analyzer_printer(void){
    return circular_buffer_analyzer_printer;
}

int circ_bbuf_push_info(circ_bbuf_info_t *c, cpu_info_t data)
{
    int next;

    next = c->head + 1;  // next is where head will point to after this write.
    if (next >= c->maxlen)
        next = 0;
    if (next == c->tail)  // if the head + 1 == tail, circular buffer is full
        return -1;
    c->buffer[c->head] = data;  // Load data and then move
    c->head = next;             // head to next data offset.
    return 0;  // return success to indicate successful push.
}

int circ_bbuf_pop_info(circ_bbuf_info_t *c, cpu_info_t *data)
{
    int next;
    if (c->head == c->tail)  // if the head == tail, we don't have any data
        return -1;
    next = c->tail + 1;  // next is where tail will point to after this read.
    if(next >= c->maxlen)
        next = 0;
    *data = c->buffer[c->tail];
    c->tail = next;
    return 0;  // return success to indicate successful push.
}