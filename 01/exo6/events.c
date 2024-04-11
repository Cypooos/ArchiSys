#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "events.h"

event_t *add_event(event_t *events,time_t date) {
    if (events) {
        events->next = add_event(events->next,date);
        return events;
    } else {
        event_t *new;
        if (new = malloc(sizeof(event_t))) {
            new->next = NULL;
            new->time = date;
            return new;
        } else {
            printf("Malloc error\n");
            exit(1); // exit with an error
        }
    }
}

void free_events(event_t *pt) {
    if (pt) {
        free_events(pt->next);
        free(pt);
    }
}

void print_events(event_t *pt) {
    if (pt) {
        struct tm* local;
        local = localtime(&(pt->time));
        printf("Event on %s", asctime(local)); 
        print_events(pt->next);
    }
}

