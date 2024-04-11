#ifndef _EVENTS_H_
#define _EVENTS_H_
#include <time.h>


typedef struct event_t {
    time_t time; // unix epoch time in second
    struct event_t* next; 
} event_t;

event_t *add_event(event_t *events,time_t date);
void free_events(event_t *pt);
void print_events(event_t *pt);

#endif