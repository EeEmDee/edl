#ifndef ACTIVEOBJECT_H
#define ACTIVEOBJECT_H

#include <stddef.h>

#include "statemachine.h"

typedef struct ExtendedEvent EEvent;
typedef struct ActiveObject ActiveObject;

#define ACTIVEOBJECT_CAST(active_object)    ((ActiveObject*)active_object)

#define CREATE_EVENT_ACTIVEOBJECT_LIST(list_name, length)	\
ActiveObject* list_name[length]

struct ExtendedEvent{
    Event super;
    ActiveObject** aos;
    uint8_t ao_num;
};

enum{
    AO_UNDEFINED,
    AO_INIT_SUCCESS,
    AO_ACTIVEOBJECT_NULL,
    AO_ACTIVEOBJECT_LIST_NULL,
    AO_STATEMACHINE_NULL,
    AO_EVENT_NULL,
    AO_EVENTQUEUE_NULL,
    AO_EVENTQUEUE_FULL,
    AO_ADD_EVENT_SUCESS,
    AO_ENQUEUE_SUCCES
};

struct ActiveObject{
	StateMachine* sm;
	Event* queue;
    uint8_t head;
    uint8_t tail;
};

extern uint8_t activeObjectInit(ActiveObject* ao, StateMachine* sm, Event* queue);
extern uint8_t activeObjectAddEventToQueue(ActiveObject* ao, Event* e);
extern Event* activeObjectRemoveEventFromQueue(ActiveObject* ao);
extern uint8_t activeObjectEventQueueIsFull(ActiveObject* ao);
extern uint8_t activeObjectEventQueueIsEmpty(ActiveObject* ao);
extern uint8_t eventEnqueue(ActiveObject** list, uint8_t length, Event* e);

#endif // ACTIVEOBJECT_H
