#include "statemachine.h"
#include "teststates.h"

#include <stdlib.h>

int state_entries[SH_MAX];
int state_exits[SH_MAX];
int state_inits[SH_MAX];
int state_transitions[SH_MAX];
int state_errors[SH_MAX];

void testStatesInitCounters(){
    for(int i = 0; i < SH_MAX; i++){
        state_entries[i] = 0;
        state_exits[i] = 0;
        state_inits[i] = 0;
        state_transitions[i] = 0;
        state_errors[i] = 0;
    }
}

State testStates_initial(HsmTst* this, Event* e){
    this->foo = 0;
	// printf("top-INIT;");
	// printf("me->foo = 0;");
    state_transitions[SH_INITIAL]++;
    return TRAN(testStates_s2);
}

State testStates_s(HsmTst* this, Event* e){
    State status_;
    switch (e->signal) {
        case ENTRY_SIG: {
            // printf("s-ENTRY;");
            state_entries[SH_S]++;
            status_ = HANDLED_STATUS;
            break;
        }
        case EXIT_SIG: {
            // printf("s-EXIT;");
            state_exits[SH_S]++;
            status_ = HANDLED_STATUS;
            break;
        }
        case INIT_SIG: {
            // printf("s-INIT;");
            state_inits[SH_S]++;
            state_transitions[SH_S]++;
            status_ = TRAN(testStates_s11);
            break;
        }
        case I_SIG: {
            if (this->foo) {
                this->foo = 0U;
                // printf("foo = 0;");
                // printf("s-I;");
                status_ = HANDLED_STATUS;
            }
            else {
                status_ = UNHANDLED_STATUS;
            }
            break;
        }
        case E_SIG: {
            // printf("s-E;");
            state_transitions[SH_S]++;
            status_ = TRAN(testStates_s11);
            break;
        }
        case TERMINATE_SIG: {
            exit(-1);
            status_ = HANDLED_STATUS;
            break;
        }
        default: {
            status_ = SUPER(stateMachineTop);
            break;
        }
    }
    return status_;
}

State testStates_s1(HsmTst* this, Event* e) {
    State status_;
    switch (e->signal) {
        case ENTRY_SIG: {
            // printf("s1-ENTRY;");
            state_entries[SH_S1]++;
            status_ = HANDLED_STATUS;
            break;
        }
        case EXIT_SIG: {
            // printf("s1-EXIT;");
            state_exits[SH_S1]++;
            status_ = HANDLED_STATUS;
            break;
        }
        case INIT_SIG: {
            // printf("s1-INIT;");
            state_inits[SH_S1]++;
            state_transitions[SH_S1]++;
            status_ = TRAN(testStates_s11);
            break;
        }
        case I_SIG: {
            // printf("s1-I;");
            status_ = HANDLED_STATUS;
            break;
        }
        case D_SIG: {
            if (!this->foo) {
                // printf("foo = 1;");
                this->foo = 1U;
                // printf("s1-D;");
                state_transitions[SH_S1]++;
                status_ = TRAN(testStates_s);
            }
            else {
                status_ = UNHANDLED_STATUS;
            }
            break;
        }
        case A_SIG: {
            // printf("s1-A;");
            state_transitions[SH_S1]++;
            status_ = TRAN(testStates_s1);
            break;
        }
        case B_SIG: {
            // printf("s1-B;");
            state_transitions[SH_S1]++;
            status_ = TRAN(testStates_s11);
            break;
        }
        case F_SIG: {
            // printf("s1-F;");
            state_transitions[SH_S1]++;
            status_ = TRAN(testStates_s211);
            break;
        }
        case C_SIG: {
            // printf("s1-C;");
            state_transitions[SH_S1]++;
            status_ = TRAN(testStates_s2);
            break;
        }
        default: {
            status_ = SUPER(testStates_s);
            break;
        }
    }
    return status_;
}
State testStates_s11(HsmTst* this, Event* e) {
    State status_;
    switch (e->signal) {
        case ENTRY_SIG: {
            // printf("s11-ENTRY;");
            state_entries[SH_S11]++;
            status_ = HANDLED_STATUS;
            break;
        }
        case EXIT_SIG: {
            // printf("s11-EXIT;");
            state_exits[SH_S11]++;
            status_ = HANDLED_STATUS;
            break;
        }
        case H_SIG: {
            // printf("s11-H;");
            state_transitions[SH_S11]++;
            status_ = TRAN(testStates_s);
            break;
        }
        case D_SIG: {
            if (this->foo) {
                // printf("foo = 0;");
                this->foo = 0U;
                // printf("s11-D;");
                state_transitions[SH_S11]++;
                status_ = TRAN(testStates_s1);
            }
            else {
                status_ = UNHANDLED_STATUS;
            }
            break;
        }
        case G_SIG: {
            // printf("s11-G;");
            state_transitions[SH_S11]++;
            status_ = TRAN(testStates_s211);
            break;
        }
        default: {
            status_ = SUPER(testStates_s1);
            break;
        }
    }
    return status_;
}
State testStates_s2(HsmTst* this, Event* e) {
    State status_;
    switch (e->signal) {
        case ENTRY_SIG: {
            // printf("s2-ENTRY;");
            state_entries[SH_S2]++;
            status_ = HANDLED_STATUS;
            break;
        }
        case EXIT_SIG: {
            // printf("s2-EXIT;");
            state_exits[SH_S2]++;
            status_ = HANDLED_STATUS;
            break;
        }
        case INIT_SIG: {
            // printf("s2-INIT;");
            state_inits[SH_S2]++;
            state_transitions[SH_S2]++;
            status_ = TRAN(testStates_s211);
            break;
        }
        case I_SIG: {
            if (!this->foo) {
                // printf("foo = 1;");
                this->foo = 1U;
                // printf("s2-I;");
                status_ = HANDLED_STATUS;
            }
            else {
                status_ = UNHANDLED_STATUS;
            }
            break;
        }
        case F_SIG: {
            // printf("s2-F;");
            state_transitions[SH_S2]++;
            status_ = TRAN(testStates_s11);
            break;
        }
        case C_SIG: {
            // printf("s2-C;");
            state_transitions[SH_S2]++;
            status_ = TRAN(testStates_s1);
            break;
        }
        default: {
            status_ = SUPER(testStates_s);
            break;
        }
    }
    return status_;
}
State testStates_s21(HsmTst* this, Event* e) {
    State status_;
    switch (e->signal) {
        case ENTRY_SIG: {
            // printf("s21-ENTRY;");
            state_entries[SH_S21]++;
            status_ = HANDLED_STATUS;
            break;
        }
        case EXIT_SIG: {
            // printf("s21-EXIT;");
            state_exits[SH_S21]++;
            status_ = HANDLED_STATUS;
            break;
        }
        case INIT_SIG: {
            // printf("s21-INIT;");
            state_inits[SH_S21]++;
            state_transitions[SH_S21]++;
            status_ = TRAN(testStates_s211);
            break;
        }
        case G_SIG: {
            // printf("s21-G;");
            state_transitions[SH_S21]++;
            status_ = TRAN(testStates_s1);
            break;
        }
        case A_SIG: {
            // printf("s21-A;");
            state_transitions[SH_S21]++;
            status_ = TRAN(testStates_s21);
            break;
        }
        case B_SIG: {
            // printf("s21-B;");
            state_transitions[SH_S21]++;
            status_ = TRAN(testStates_s211);
            break;
        }
        default: {
            status_ = SUPER(testStates_s2);
            break;
        }
    }
    return status_;
}
State testStates_s211(HsmTst* this, Event* e) {
    State status_;
    switch (e->signal) {
        case ENTRY_SIG: {
            // printf("s211-ENTRY;");
            state_entries[SH_S211]++;
            status_ = HANDLED_STATUS;
            break;
        }
        case EXIT_SIG: {
            // printf("s211-EXIT;");
            state_exits[SH_S211]++;
            status_ = HANDLED_STATUS;
            break;
        }
        case H_SIG: {
            // printf("s211-H;");
            state_transitions[SH_S211]++;
            status_ = TRAN(testStates_s);
            break;
        }
        case D_SIG: {
            // printf("s211-D;");
            state_transitions[SH_S211]++;
            status_ = TRAN(testStates_s21);
            break;
        }
        default: {
            status_ = SUPER(testStates_s21);
            break;
        }
    }
    return status_;
}