#ifndef QUEUE_H
#define QUEUE_H
#include "reader.h"

typedef struct {
    cpu_info_t* const buffer;
    int head;
    int tail;
    const int maxlen;
} circ_bbuf_info_t;

extern circ_bbuf_info_t circular_buffer_reader_analyzer;
extern circ_bbuf_info_t circular_buffer_analyzer_printer;
int circ_bbuf_push_info(circ_bbuf_info_t *c, cpu_info_t data);
int circ_bbuf_pop_info(circ_bbuf_info_t *c, cpu_info_t *data);

#endif // QUEUE_H