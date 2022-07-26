#include "activeobject.h"

#include <stdio.h>

static uint8_t activeObjectEventQueueNextIndex(uint8_t index){
    index++;
    if(index >= EVENT_BUFFER_LENGTH+1) index = 0;
    return index;
}

uint8_t activeObjectEventQueueIsFull(ActiveObject* ao){
    return activeObjectEventQueueNextIndex(ao->head) == ao->tail;
}

uint8_t activeObjectEventQueueIsEmpty(ActiveObject* ao){
    return (ao->head == ao->tail);
}

uint8_t activeObjectAddEventToQueue(ActiveObject* ao, Event* e){
    if(activeObjectEventQueueIsFull(ao)) return AO_EVENTQUEUE_FULL;
    
    printf("e->signal: %d\n", e->signal);
    ao->queue[ao->head].signal = e->signal;
    
    ao->head = activeObjectEventQueueNextIndex(ao->head);

    return AO_ADD_EVENT_SUCESS;
}

Event* activeObjectRemoveEventFromQueue(ActiveObject* ao){
    Event* element = NULL;
    if(ao == NULL) return element;
    if(activeObjectEventQueueIsEmpty(ao)) return element;

    element = &ao->queue[ao->tail];

    ao->tail = activeObjectEventQueueNextIndex(ao->tail);

    return element;
}

uint8_t activeObjectInit(ActiveObject* ao, StateMachine* sm, Event* queue){
    if(ao == NULL) return AO_ACTIVEOBJECT_NULL;
    if(sm == NULL) return AO_STATEMACHINE_NULL;
    if(queue == NULL) return AO_EVENTQUEUE_NULL;
    
    ao->sm = sm;
    ao->queue = queue;
    ao->head = 0;
    ao->tail = 0;

    return AO_INIT_SUCCESS;
}

uint8_t eventEnqueue(ActiveObject** ao_list, uint8_t length, Event* e){

    if(ao_list == NULL) return  AO_ACTIVEOBJECT_LIST_NULL;
    if(e == NULL) return AO_EVENT_NULL;

    for(int i = 0; i < length; i++){
        if(!activeObjectEventQueueIsFull(ao_list[i])){
            activeObjectAddEventToQueue(ao_list[i], e);
        }
        else{
            return AO_EVENTQUEUE_FULL;
        }
    }
    return AO_ENQUEUE_SUCCES;
}