#include "queue.h"

#define QUEUE_SIZE      8
#define QUEUE_SIZE_MASK (QUEUE_SIZE - 1)

static volatile uint8_t sysEventQueueHead, sysEventQueueTail = 0 ;
static volatile uint8_t sysEventQueue[QUEUE_SIZE];

void queueEvent(uint8_t eventcode) {
    sysEventQueueHead = (sysEventQueueHead + 1) & QUEUE_SIZE_MASK;
    if (sysEventQueueHead == sysEventQueueTail) sysEventQueueTail = (sysEventQueueTail + 1) & QUEUE_SIZE_MASK;
    sysEventQueue[sysEventQueueHead] = eventcode;
}

uint8_t getLastEvent(void) {
    if (sysEventQueueHead == sysEventQueueTail) return 0;
    sysEventQueueTail = (sysEventQueueTail + 1) & QUEUE_SIZE_MASK;
    return sysEventQueue[sysEventQueueTail];
}