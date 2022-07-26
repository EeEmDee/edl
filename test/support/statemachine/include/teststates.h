#ifndef _TESTSTATES_H_
#define _TESTSTATES_H_

#include <stdint.h>
#include "statemachine.h"

enum HsmTstSignals {
    A_SIG = USER_SIG,
    B_SIG,
    C_SIG,
    D_SIG,
    E_SIG,
    F_SIG,
    G_SIG,
    H_SIG,
    I_SIG,
    TERMINATE_SIG,
    IGNORE_SIG,
    MAX_SIG
};

enum StateHandlerNames{
    SH_INITIAL,
    SH_S,
    SH_S1,
    SH_S11,
    SH_S2,
    SH_S21,
    SH_S211,
    SH_MAX
};

typedef struct {
/* protected: */
    StateMachine super;

/* private: */
    uint8_t foo;
} HsmTst;

extern int state_entries[];
extern int state_exits[];
extern int state_inits[];
extern int state_transitions[];
extern int state_errors[];

extern void testStatesInitCounters(void);

extern State testStates_initial(HsmTst* this, Event* e);    //  0
extern State testStates_s(HsmTst* this, Event* e);          //  1
extern State testStates_s1(HsmTst* this, Event* e);         //  2
extern State testStates_s11(HsmTst* this, Event* e);        //  3
extern State testStates_s2(HsmTst* this, Event* e);         //  4
extern State testStates_s21(HsmTst* this, Event* e);        //  5
extern State testStates_s211(HsmTst* this, Event* e);       //  6

#endif