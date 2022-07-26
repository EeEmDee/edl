#ifndef _STATEMACHINE_H_
#define _STATEMACHINE_H_

#include <stdint.h>

#define EVENT_BUFFER_LENGTH 32

typedef struct Event Event;
typedef struct StateMachine StateMachine;

/**************************************
---------> Event Definitions <---------
***************************************/
#define EVENT_CAST(e)  ((Event*)e)

struct Event{
    uint8_t signal;        // Event Signal
};

enum ReservedSignals {
    INIT_SIG,   /* for triggering the init action in a state */
    ENTRY_SIG,  /* for triggering the entry action in a state */
    EXIT_SIG,   /* for triggering the exit action from a state */
    EMPTY_SIG,   /* for triggering the empty action from a state */
    USER_SIG    /* first signal available to the users */
};

extern Event initEvent;
extern Event entryEvent;
extern Event exitEvent;
extern Event emptyEvent;
extern Event* nullEvent;

/**************************************
------> Statemachine Definitions <------
***************************************/
#define STATEHANDLER_CAST(handler)  ((StateHandler*)handler)

#define STATEMACHINE_MAXIMUM_NEST_DEPTH  6

typedef enum {
    TRAN_STATUS,
    HANDLED_STATUS,
    IGNORED_STATUS,
    SUPER_STATUS,
    UNHANDLED_STATUS,
    ERROR_STATUS,
    INIT_STATUS
}State;

// Function pointer to handle a state machine event
typedef State (*StateHandler)(StateMachine* sm, Event* e);

struct StateMachine{
	StateHandler state;
	StateHandler temp;
};

#define TRAN(target_) (((StateMachine *)this)->temp = (StateHandler)(target_), TRAN_STATUS)
#define SUPER(super_) (((StateMachine *)this)->temp = (StateHandler)(super_), SUPER_STATUS)

// Function declarations
void stateMachineInit(StateMachine* sm, StateHandler* init_handler);
void stateMachineStart(StateMachine* sm);
void stateMachineDispatch(StateMachine* sm, Event* e);
State stateMachineTop(StateMachine* sm, Event* e);
State stateMachineError(StateMachine* sm, Event* e);

#endif
