#ifndef TEST
#define TEST
#endif

#ifdef TEST

#ifndef uint8_t
typedef unsigned char uint8_t;
#endif

#include "unity.h"

#include "statemachine.h"
#include "activeobject.h"

#define EVENT_AO_LIST_LENGTH    1

CREATE_EVENT_ACTIVEOBJECT_LIST(event_aos, EVENT_AO_LIST_LENGTH);

typedef struct{
    EEvent super;

    // Dummy variable
    uint8_t dummy;
}TestEvent;

EEvent event_buffer[EVENT_BUFFER_LENGTH+1];
TestEvent event;
StateMachine sm;
ActiveObject ao;

static uint8_t init_status = AO_UNDEFINED;

void setUp(void){
    event_aos[0] = ACTIVEOBJECT_CAST(&ao);
    event.super.aos = event_aos;
    event.super.ao_num = EVENT_AO_LIST_LENGTH;
    init_status = activeObjectInit(&ao, &sm, EVENT_CAST(event_buffer));
}

void tearDown(void){
}

void test_activeobject_initializationFailsWhenActiveObjectIsNullPointer(void){
    uint8_t ret = activeObjectInit(NULL, &sm, EVENT_CAST(event_buffer));
    TEST_ASSERT_EQUAL_UINT8(AO_ACTIVEOBJECT_NULL, ret);
}

void test_activeobject_initializationFailsWhenStateMachineIsNullPointer(void){
    uint8_t ret = activeObjectInit(&ao, NULL, EVENT_CAST(event_buffer));
    TEST_ASSERT_EQUAL_UINT8(AO_STATEMACHINE_NULL, ret);
}

void test_activeobject_initializationFailsWhenEventQueueIsNullPointer(void){
    uint8_t ret = activeObjectInit(&ao, &sm, NULL);
    TEST_ASSERT_EQUAL_UINT8(AO_EVENTQUEUE_NULL, ret);
}

void test_activeobject_initializationSuccessfull(void){
    TEST_ASSERT_EQUAL_UINT8(AO_INIT_SUCCESS, init_status);
    TEST_ASSERT_EQUAL_PTR(EVENT_CAST(event_buffer), ao.queue);
    TEST_ASSERT_EQUAL_PTR(&sm, ao.sm);
    TEST_ASSERT_EQUAL_UINT8(0, ao.head);
    TEST_ASSERT_EQUAL_UINT8(0, ao.tail);
}

void test_activeobject_activeObjectIsFullIsTrueWhenFull(void){
    uint8_t ret = AO_UNDEFINED;
    for(int i = 0; i < EVENT_BUFFER_LENGTH; i++){
        activeObjectAddEventToQueue(&ao, &emptyEvent);
    }
    ret = activeObjectAddEventToQueue(&ao, &emptyEvent);
    TEST_ASSERT_EQUAL_UINT8(AO_EVENTQUEUE_FULL, ret);
    TEST_ASSERT_EQUAL_UINT8(1, activeObjectEventQueueIsFull(&ao));
}

void test_activeobject_activeObjectIsFullIsFalseWhenNotFull(void){
    uint8_t ret = AO_UNDEFINED;
    ret = activeObjectAddEventToQueue(&ao, &emptyEvent);
    TEST_ASSERT_EQUAL_UINT8(AO_ADD_EVENT_SUCESS, ret);
    TEST_ASSERT_EQUAL_UINT8(0, activeObjectEventQueueIsFull(&ao));
}

void test_activeobject_activeObjectIsEmptyIsTrueWhenEmpty(void){
    TEST_ASSERT_EQUAL_UINT8(1, activeObjectEventQueueIsEmpty(&ao));
}

void test_activeobject_activeObjectIsEmptyIsFalseWhenNotEmpty(void){
    uint8_t ret = AO_UNDEFINED;
    ret = activeObjectAddEventToQueue(&ao, &emptyEvent);
    TEST_ASSERT_EQUAL_UINT8(AO_ADD_EVENT_SUCESS, ret);
    TEST_ASSERT_EQUAL_UINT8(0, activeObjectEventQueueIsEmpty(&ao));
}

void test_activeobject_activeObjectRemoveEventFromQueueRejectsNullActiveObject(void){
    Event* e = activeObjectRemoveEventFromQueue(NULL);
    TEST_ASSERT_EQUAL_PTR(NULL, e);
}

void test_activeobject_activeObjectRemoveEventFromQueueReturnsNullWhenQueueEmpty(void){
    Event* e = activeObjectRemoveEventFromQueue(&ao);
    TEST_ASSERT_EQUAL_PTR(NULL, e);
}

void test_activeobject_activeObjectRemoveEventFromQueueReturnsFirstAddedEvent(void){
    eventEnqueue(event.super.aos, event.super.ao_num, &initEvent);
    eventEnqueue(event.super.aos, event.super.ao_num, &entryEvent);

    Event* e = activeObjectRemoveEventFromQueue(&ao);

    TEST_ASSERT_EQUAL_UINT8(initEvent.signal, e->signal);
}

void test_activeobject_activeObjectRemoveEventFromQueueReturnsAddedEventsInOrder(void){
    eventEnqueue(event.super.aos, event.super.ao_num, &initEvent);
    eventEnqueue(event.super.aos, event.super.ao_num, &entryEvent);
    eventEnqueue(event.super.aos, event.super.ao_num, &exitEvent);
    eventEnqueue(event.super.aos, event.super.ao_num, &emptyEvent);

    Event* e = activeObjectRemoveEventFromQueue(&ao);
    TEST_ASSERT_EQUAL_UINT8(initEvent.signal, e->signal);
    e = activeObjectRemoveEventFromQueue(&ao);
    TEST_ASSERT_EQUAL_UINT8(entryEvent.signal, e->signal);
    e = activeObjectRemoveEventFromQueue(&ao);
    TEST_ASSERT_EQUAL_UINT8(exitEvent.signal, e->signal);
    e = activeObjectRemoveEventFromQueue(&ao);
    TEST_ASSERT_EQUAL_UINT8(emptyEvent.signal, e->signal);
}

void test_activeobject_eventEnqueueRejectsNullActiveObjectList(void){
    uint8_t ret = eventEnqueue(NULL, event.super.ao_num, &initEvent);
    TEST_ASSERT_EQUAL_UINT8(AO_ACTIVEOBJECT_LIST_NULL, ret);
}

void test_activeobject_eventEnqueueRejectsNullEvent(void){
    uint8_t ret = eventEnqueue(event.super.aos, event.super.ao_num, nullEvent);
    TEST_ASSERT_EQUAL_UINT8(AO_EVENT_NULL, ret);
}

void test_activeobject_eventEnqueueSuccess(void){
    uint8_t ret = eventEnqueue(event.super.aos, event.super.ao_num, &initEvent);
    TEST_ASSERT_EQUAL_UINT8(AO_ENQUEUE_SUCCES, ret);
}

void test_activeobject_eventEnqueueReturnsFullWhenFull(void){
    uint8_t ret = AO_UNDEFINED;
    for(int i = 0; i < EVENT_BUFFER_LENGTH; i++){
        eventEnqueue(event.super.aos, event.super.ao_num, &initEvent);
    }
    ret = eventEnqueue(event.super.aos, event.super.ao_num, &initEvent);
    TEST_ASSERT_EQUAL_UINT8(AO_EVENTQUEUE_FULL, ret);
}

#endif // TEST
