#include <stdio.h>
#include <time.h>
#include "events.h"


int main() {
    event_t *calendar = add_event(add_event(add_event(NULL,1707406925),1707579725),1707667200);
    print_events(calendar);
    free_events(calendar);
    return 0;
}