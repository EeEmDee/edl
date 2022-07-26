#ifndef TEST
#define TEST
#endif

#ifdef TEST

#include "unity.h"

#include "statemachine.h"
#include "teststates.h"

HsmTst state_machine;

static void printStateArray(int* arr, const char* type){
    printf("%s:\n", type);
    for(int i = 0; i < SH_MAX; i++){
        if(i < 10) printf(" %d ", i);
        else printf(" %d", i);
    }
    for(int i = 0; i < SH_MAX; i++){
        printf("[%d]", arr[i]);
    }
    printf("\n\n");
}

static void printStateArrays(void){
    printStateArray(state_entries, "State Entrys");
    printStateArray(state_inits, "State Inits");
    printStateArray(state_exits, "State Exits");
    printStateArray(state_transitions, "State Transitions");
}

static void printStatePointers(void){
    printf("stateMachineTop:\t%p\n", stateMachineTop);
    printf("stateMachineError:\t%p\n", stateMachineError);
    printf("initial:\t%p\n", testStates_initial);
    printf("s:\t\t%p\n", testStates_s);
    printf("s1:\t%p\n", testStates_s1);
    printf("s11:\t%p\n", testStates_s11);
    printf("s2:\t%p\n", testStates_s2);
    printf("s21:\t%p\n", testStates_s21);
    printf("s211:\t%p\n\n", testStates_s211);
}

void setUp(void){
    stateMachineInit(&state_machine.super, STATEHANDLER_CAST(testStates_initial));
    testStatesInitCounters();
}

void tearDown(void){
}

void test_statemachine_topStateReturnsIgnored(void){
    TEST_ASSERT_EQUAL(IGNORED_STATUS, stateMachineTop(&state_machine.super, nullEvent));
}

void test_statemachine_initSetsCorrectStateHandlers(void){
    TEST_ASSERT_EQUAL_PTR(stateMachineTop, state_machine.super.state);
    TEST_ASSERT_EQUAL_PTR(testStates_initial, state_machine.super.temp);
}

void test_statemachine_stateCountersInitializedCorrectly(void){
    TEST_ASSERT_EACH_EQUAL_INT(0, state_entries, SH_MAX);
    TEST_ASSERT_EACH_EQUAL_INT(0, state_exits, SH_MAX);
    TEST_ASSERT_EACH_EQUAL_INT(0, state_inits, SH_MAX);
    TEST_ASSERT_EACH_EQUAL_INT(0, state_transitions, SH_MAX);
    TEST_ASSERT_EACH_EQUAL_INT(0, state_errors, SH_MAX);
}

// top-INIT;foo = 0;s-ENTRY;s2-ENTRY;s2-INIT;s21-ENTRY;s211-ENTRY;
void test_statemachine_startStateMachineAndTransitionToCorrectHandler(void){
    stateMachineStart(&state_machine.super);
    TEST_ASSERT_EQUAL_PTR(testStates_s211, state_machine.super.state);

    TEST_ASSERT_EQUAL_INT(1, state_entries[SH_S]);
    TEST_ASSERT_EQUAL_INT(1, state_entries[SH_S2]);
    TEST_ASSERT_EQUAL_INT(1, state_entries[SH_S21]);
    TEST_ASSERT_EQUAL_INT(1, state_entries[SH_S211]);

    TEST_ASSERT_EQUAL_INT(1, state_inits[SH_S2]);

    TEST_ASSERT_EQUAL_INT(1, state_transitions[SH_INITIAL]);
}

// s21-A;s211-EXIT;s21-EXIT;s21-ENTRY;s21-INIT;s211-ENTRY;
void test_statemachine_S211WithASigTransitionsToS211(void){
    Event e;
    e.signal = A_SIG;

    stateMachineStart(&state_machine.super);

    testStatesInitCounters();

    stateMachineDispatch(&state_machine.super, &e);

    TEST_ASSERT_EQUAL_PTR(testStates_s211, state_machine.super.state);

    TEST_ASSERT_EQUAL_INT(1, state_entries[SH_S21]);
    TEST_ASSERT_EQUAL_INT(1, state_entries[SH_S211]);

    TEST_ASSERT_EQUAL_INT(1, state_inits[SH_S21]);

    TEST_ASSERT_EQUAL_INT(1, state_exits[SH_S21]);
    TEST_ASSERT_EQUAL_INT(1, state_exits[SH_S211]);
    
    TEST_ASSERT_EQUAL_INT(2, state_transitions[SH_S21]);

}

// s21-B;s211-EXIT;s211-ENTRY;
void test_statemachine_S211WithBSigTransitionsToS211(void){
    Event e;
    e.signal = B_SIG;

    stateMachineStart(&state_machine.super);

    testStatesInitCounters();

    stateMachineDispatch(&state_machine.super, &e);
    
    TEST_ASSERT_EQUAL_PTR(testStates_s211, state_machine.super.state);
    TEST_ASSERT_EQUAL_INT(1, state_entries[SH_S211]);
    
    TEST_ASSERT_EQUAL_INT(1, state_exits[SH_S211]);

    TEST_ASSERT_EQUAL_INT(1, state_transitions[SH_S21]);

}

// s2-C;s211-EXIT;s21-EXIT;s2-EXIT;s1-ENTRY;s1-INIT;s11-ENTRY;
void test_statemachine_S211WithCSigTransitionsToS11(void){
    Event e;
    e.signal = C_SIG;

    stateMachineStart(&state_machine.super);
    testStatesInitCounters();
    stateMachineDispatch(&state_machine.super, &e);
    
    TEST_ASSERT_EQUAL_PTR(testStates_s11, state_machine.super.state);
    
    TEST_ASSERT_EQUAL_INT(1, state_entries[SH_S1]);
    TEST_ASSERT_EQUAL_INT(1, state_entries[SH_S11]);
    
    TEST_ASSERT_EQUAL_INT(1, state_inits[SH_S1]);

    TEST_ASSERT_EQUAL_INT(1, state_exits[SH_S2]);
    TEST_ASSERT_EQUAL_INT(1, state_exits[SH_S21]);
    TEST_ASSERT_EQUAL_INT(1, state_exits[SH_S211]);

    TEST_ASSERT_EQUAL_INT(1, state_transitions[SH_S1]);
    TEST_ASSERT_EQUAL_INT(1, state_transitions[SH_S2]);


}

// s211-D;s211-EXIT;s21-INIT;s211-ENTRY;
void test_statemachine_S211WithDSigTransitionsToS211(void){
    Event e;
    e.signal = D_SIG;

    stateMachineStart(&state_machine.super);
    testStatesInitCounters();
    stateMachineDispatch(&state_machine.super, &e);
    
    TEST_ASSERT_EQUAL_PTR(testStates_s211, state_machine.super.state);

    TEST_ASSERT_EQUAL_INT(1, state_entries[SH_S211]);

    TEST_ASSERT_EQUAL_INT(1, state_inits[SH_S21]);
    
    TEST_ASSERT_EQUAL_INT(1, state_exits[SH_S211]);

    TEST_ASSERT_EQUAL_INT(1, state_transitions[SH_S21]);
    TEST_ASSERT_EQUAL_INT(1, state_transitions[SH_S211]);

}

// s-E;s211-EXIT;s21-EXIT;s2-EXIT;s1-ENTRY;s11-ENTRY;
void test_statemachine_S211WithESigTransitionsToS11(void){
    Event e;
    e.signal = E_SIG;

    stateMachineStart(&state_machine.super);
    testStatesInitCounters();
    stateMachineDispatch(&state_machine.super, &e);
    
    TEST_ASSERT_EQUAL_PTR(testStates_s11, state_machine.super.state);

    TEST_ASSERT_EQUAL_INT(1, state_entries[SH_S1]);
    TEST_ASSERT_EQUAL_INT(1, state_entries[SH_S11]);

    TEST_ASSERT_EQUAL_INT(1, state_exits[SH_S2]);
    TEST_ASSERT_EQUAL_INT(1, state_exits[SH_S21]);
    TEST_ASSERT_EQUAL_INT(1, state_exits[SH_S211]);

    TEST_ASSERT_EQUAL_INT(1, state_transitions[SH_S]);

}

// s2-F;s211-EXIT;s21-EXIT;s2-EXIT;s1-ENTRY;s11-ENTRY;
void test_statemachine_S211WithFSigTransitionsToS11(void){
    Event e;
    e.signal = F_SIG;

    stateMachineStart(&state_machine.super);
    testStatesInitCounters();
    stateMachineDispatch(&state_machine.super, &e);
    
    TEST_ASSERT_EQUAL_PTR(testStates_s11, state_machine.super.state);

    TEST_ASSERT_EQUAL_INT(1, state_entries[SH_S1]);
    TEST_ASSERT_EQUAL_INT(1, state_entries[SH_S11]);

    TEST_ASSERT_EQUAL_INT(1, state_exits[SH_S2]);
    TEST_ASSERT_EQUAL_INT(1, state_exits[SH_S21]);
    TEST_ASSERT_EQUAL_INT(1, state_exits[SH_S211]);

    TEST_ASSERT_EQUAL_INT(1, state_transitions[SH_S2]);

}

// s21-G;s211-EXIT;s21-EXIT;s2-EXIT;s1-ENTRY;s1-INIT;s11-ENTRY;
void test_statemachine_S211WithGSigTransitionsToS11(void){
    Event e;
    e.signal = G_SIG;

    stateMachineStart(&state_machine.super);
    testStatesInitCounters();
    stateMachineDispatch(&state_machine.super, &e);
    
    TEST_ASSERT_EQUAL_PTR(testStates_s11, state_machine.super.state);

    TEST_ASSERT_EQUAL_INT(1, state_entries[SH_S1]);
    TEST_ASSERT_EQUAL_INT(1, state_entries[SH_S11]);

    TEST_ASSERT_EQUAL_INT(1, state_inits[SH_S1]);
    
    TEST_ASSERT_EQUAL_INT(1, state_exits[SH_S2]);
    TEST_ASSERT_EQUAL_INT(1, state_exits[SH_S21]);
    TEST_ASSERT_EQUAL_INT(1, state_exits[SH_S211]);

    TEST_ASSERT_EQUAL_INT(1, state_transitions[SH_S1]);
    TEST_ASSERT_EQUAL_INT(1, state_transitions[SH_S21]);


}

// s211-H;s211-EXIT;s21-EXIT;s2-EXIT;s-INIT;s1-ENTRY;s11-ENTRY;
void test_statemachine_S211WithHSigTransitionsToS11(void){
    Event e;
    e.signal = H_SIG;

    stateMachineStart(&state_machine.super);
    testStatesInitCounters();
    stateMachineDispatch(&state_machine.super, &e);
    
    TEST_ASSERT_EQUAL_PTR(testStates_s11, state_machine.super.state);
    
    TEST_ASSERT_EQUAL_INT(1, state_entries[SH_S1]);
    TEST_ASSERT_EQUAL_INT(1, state_entries[SH_S11]);

    TEST_ASSERT_EQUAL_INT(1, state_inits[SH_S]);
    
    TEST_ASSERT_EQUAL_INT(1, state_exits[SH_S2]);
    TEST_ASSERT_EQUAL_INT(1, state_exits[SH_S21]);
    TEST_ASSERT_EQUAL_INT(1, state_exits[SH_S211]);

    TEST_ASSERT_EQUAL_INT(1, state_transitions[SH_S]);
    TEST_ASSERT_EQUAL_INT(1, state_transitions[SH_S211]);
    
}

// foo = 1;s2-I;
void test_statemachine_S211WithISigTransitionsToS211AndSetFooToOne(void){
    Event e;
    e.signal = I_SIG;

    stateMachineStart(&state_machine.super);
    testStatesInitCounters();
    stateMachineDispatch(&state_machine.super, &e);
    
    TEST_ASSERT_EQUAL_PTR(testStates_s211, state_machine.super.state);
    TEST_ASSERT_EQUAL_UINT8(1, state_machine.foo);

    TEST_ASSERT_EACH_EQUAL_INT(0, state_entries, SH_MAX);
    TEST_ASSERT_EACH_EQUAL_INT(0, state_inits, SH_MAX);
    TEST_ASSERT_EACH_EQUAL_INT(0, state_exits, SH_MAX);
    TEST_ASSERT_EACH_EQUAL_INT(0, state_transitions, SH_MAX);

}

// foo = 0;s-I;
void test_statemachine_S211WithTwoISigTransitionsToS211AndSetFooToZero(void){
    Event e;
    e.signal = I_SIG;

    stateMachineStart(&state_machine.super);
    testStatesInitCounters();
    stateMachineDispatch(&state_machine.super, &e);
    stateMachineDispatch(&state_machine.super, &e);

    TEST_ASSERT_EQUAL_PTR(testStates_s211, state_machine.super.state);
    TEST_ASSERT_EQUAL_UINT8(0, state_machine.foo);

    TEST_ASSERT_EACH_EQUAL_INT(0, state_entries, SH_MAX);
    TEST_ASSERT_EACH_EQUAL_INT(0, state_inits, SH_MAX);
    TEST_ASSERT_EACH_EQUAL_INT(0, state_exits, SH_MAX);
    TEST_ASSERT_EACH_EQUAL_INT(0, state_transitions, SH_MAX);

}

// s1-A;s11-EXIT;s1-EXIT;s1-ENTRY;s1-INIT;s11-ENTRY;
void test_statemachine_S11WithASigTransitionToS11(void){
    Event e;
    e.signal = C_SIG;

    stateMachineStart(&state_machine.super);
    stateMachineDispatch(&state_machine.super, &e);

    testStatesInitCounters();

    e.signal = A_SIG;
    stateMachineDispatch(&state_machine.super, &e);

    TEST_ASSERT_EQUAL_PTR(testStates_s11, state_machine.super.state);

    TEST_ASSERT_EQUAL_INT(1, state_entries[SH_S1]);
    TEST_ASSERT_EQUAL_INT(1, state_entries[SH_S11]);

    TEST_ASSERT_EQUAL_INT(1, state_inits[SH_S1]);
    
    TEST_ASSERT_EQUAL_INT(1, state_exits[SH_S1]);
    TEST_ASSERT_EQUAL_INT(1, state_exits[SH_S11]);

    TEST_ASSERT_EQUAL_INT(2, state_transitions[SH_S1]);
}

// s1-B;s11-EXIT;s11-ENTRY;
void test_statemachine_S11WithBSigTransitionToS11(void){
    Event e;
    e.signal = C_SIG;

    stateMachineStart(&state_machine.super);
    stateMachineDispatch(&state_machine.super, &e);

    testStatesInitCounters();

    e.signal = B_SIG;
    stateMachineDispatch(&state_machine.super, &e);

    TEST_ASSERT_EQUAL_PTR(testStates_s11, state_machine.super.state);

    TEST_ASSERT_EQUAL_INT(1, state_entries[SH_S11]);

    TEST_ASSERT_EQUAL_INT(1, state_exits[SH_S11]);

    TEST_ASSERT_EQUAL_INT(1, state_transitions[SH_S1]);
}

// s1-C;s11-EXIT;s1-EXIT;s2-ENTRY;s2-INIT;s21-ENTRY;s211-ENTRY;
void test_statemachine_S11WithCSigTransitionToS211(void){
    Event e;
    e.signal = C_SIG;

    stateMachineStart(&state_machine.super);
    stateMachineDispatch(&state_machine.super, &e);

    testStatesInitCounters();

    e.signal = C_SIG;
    stateMachineDispatch(&state_machine.super, &e);

    TEST_ASSERT_EQUAL_PTR(testStates_s211, state_machine.super.state);

    TEST_ASSERT_EQUAL_INT(1, state_entries[SH_S2]);
    TEST_ASSERT_EQUAL_INT(1, state_entries[SH_S21]);
    TEST_ASSERT_EQUAL_INT(1, state_entries[SH_S211]);

    TEST_ASSERT_EQUAL_INT(1, state_inits[SH_S2]);

    TEST_ASSERT_EQUAL_INT(1, state_exits[SH_S1]);
    TEST_ASSERT_EQUAL_INT(1, state_exits[SH_S11]);

    TEST_ASSERT_EQUAL_INT(1, state_transitions[SH_S1]);
    TEST_ASSERT_EQUAL_INT(1, state_transitions[SH_S2]);
}

// foo = 1;s1-D;s11-EXIT;s1-EXIT;s-INIT;s1-ENTRY;s11-ENTRY;
void test_statemachine_S11WithDSigTransitionToS11(void){
    Event e;
    e.signal = C_SIG;

    stateMachineStart(&state_machine.super);
    stateMachineDispatch(&state_machine.super, &e);

    testStatesInitCounters();

    e.signal = D_SIG;
    stateMachineDispatch(&state_machine.super, &e);

    TEST_ASSERT_EQUAL_PTR(testStates_s11, state_machine.super.state);
    TEST_ASSERT_EQUAL_INT(1, state_machine.foo);

    TEST_ASSERT_EQUAL_INT(1, state_entries[SH_S1]);
    TEST_ASSERT_EQUAL_INT(1, state_entries[SH_S11]);

    TEST_ASSERT_EQUAL_INT(1, state_inits[SH_S]);

    TEST_ASSERT_EQUAL_INT(1, state_exits[SH_S1]);
    TEST_ASSERT_EQUAL_INT(1, state_exits[SH_S11]);

    TEST_ASSERT_EQUAL_INT(1, state_transitions[SH_S]);
    TEST_ASSERT_EQUAL_INT(1, state_transitions[SH_S1]);
}

// foo = 0;s11-D;s11-EXIT;s1-INIT;s11-ENTRY;
void test_statemachine_S11WithTwoDSigTransitionToS11(void){
    Event e;
    e.signal = C_SIG;

    stateMachineStart(&state_machine.super);
    stateMachineDispatch(&state_machine.super, &e);

     e.signal = D_SIG;
    stateMachineDispatch(&state_machine.super, &e);

    testStatesInitCounters();

    stateMachineDispatch(&state_machine.super, &e);

    TEST_ASSERT_EQUAL_PTR(testStates_s11, state_machine.super.state);
    TEST_ASSERT_EQUAL_INT(0, state_machine.foo);

    TEST_ASSERT_EQUAL_INT(1, state_entries[SH_S11]);

    TEST_ASSERT_EQUAL_INT(1, state_inits[SH_S1]);

    TEST_ASSERT_EQUAL_INT(1, state_exits[SH_S11]);

    TEST_ASSERT_EQUAL_INT(1, state_transitions[SH_S1]);
    TEST_ASSERT_EQUAL_INT(1, state_transitions[SH_S11]);
}

// s-E;s11-EXIT;s1-EXIT;s1-ENTRY;s11-ENTRY;
void test_statemachine_S11WithESigTransitionToS11(void){
    Event e;
    e.signal = C_SIG;

    stateMachineStart(&state_machine.super);
    stateMachineDispatch(&state_machine.super, &e);

    testStatesInitCounters();

    e.signal = E_SIG;
    stateMachineDispatch(&state_machine.super, &e);

    TEST_ASSERT_EQUAL_PTR(testStates_s11, state_machine.super.state);

    TEST_ASSERT_EQUAL_INT(1, state_entries[SH_S1]);
    TEST_ASSERT_EQUAL_INT(1, state_entries[SH_S11]);

    TEST_ASSERT_EQUAL_INT(1, state_exits[SH_S1]);
    TEST_ASSERT_EQUAL_INT(1, state_exits[SH_S11]);

    TEST_ASSERT_EQUAL_INT(1, state_transitions[SH_S]);
}

// s1-F;s11-EXIT;s1-EXIT;s2-ENTRY;s21-ENTRY;s211-ENTRY;
void test_statemachine_S11WithFSigTransitionToS211(void){
    Event e;
    e.signal = C_SIG;

    stateMachineStart(&state_machine.super);
    stateMachineDispatch(&state_machine.super, &e);

    testStatesInitCounters();

    e.signal = F_SIG;
    stateMachineDispatch(&state_machine.super, &e);

    TEST_ASSERT_EQUAL_PTR(testStates_s211, state_machine.super.state);

    TEST_ASSERT_EQUAL_INT(1, state_entries[SH_S2]);
    TEST_ASSERT_EQUAL_INT(1, state_entries[SH_S21]);
    TEST_ASSERT_EQUAL_INT(1, state_entries[SH_S211]);

    TEST_ASSERT_EQUAL_INT(1, state_exits[SH_S1]);
    TEST_ASSERT_EQUAL_INT(1, state_exits[SH_S11]);

    TEST_ASSERT_EQUAL_INT(1, state_transitions[SH_S1]);
}

// s11-G;s11-EXIT;s1-EXIT;s2-ENTRY;s21-ENTRY;s211-ENTRY;
void test_statemachine_S11WithGSigTransitionToS211(void){
    Event e;
    e.signal = C_SIG;

    stateMachineStart(&state_machine.super);
    stateMachineDispatch(&state_machine.super, &e);

    testStatesInitCounters();

    e.signal = G_SIG;
    stateMachineDispatch(&state_machine.super, &e);

    TEST_ASSERT_EQUAL_PTR(testStates_s211, state_machine.super.state);

    TEST_ASSERT_EQUAL_INT(1, state_entries[SH_S2]);
    TEST_ASSERT_EQUAL_INT(1, state_entries[SH_S21]);
    TEST_ASSERT_EQUAL_INT(1, state_entries[SH_S211]);

    TEST_ASSERT_EQUAL_INT(1, state_exits[SH_S1]);
    TEST_ASSERT_EQUAL_INT(1, state_exits[SH_S11]);

    TEST_ASSERT_EQUAL_INT(1, state_transitions[SH_S11]);
}

// s11-H;s11-EXIT;s1-EXIT;s-INIT;s1-ENTRY;s11-ENTRY;
void test_statemachine_S11WithHSigTransitionToS11(void){
    Event e;
    e.signal = C_SIG;

    stateMachineStart(&state_machine.super);
    stateMachineDispatch(&state_machine.super, &e);

    testStatesInitCounters();

    e.signal = H_SIG;
    stateMachineDispatch(&state_machine.super, &e);

    TEST_ASSERT_EQUAL_PTR(testStates_s11, state_machine.super.state);

    TEST_ASSERT_EQUAL_INT(1, state_entries[SH_S1]);
    TEST_ASSERT_EQUAL_INT(1, state_entries[SH_S11]);

    TEST_ASSERT_EQUAL_INT(1, state_inits[SH_S]);

    TEST_ASSERT_EQUAL_INT(1, state_exits[SH_S1]);
    TEST_ASSERT_EQUAL_INT(1, state_exits[SH_S11]);

    TEST_ASSERT_EQUAL_INT(1, state_transitions[SH_S]);
    TEST_ASSERT_EQUAL_INT(1, state_transitions[SH_S11]);
}

// s1-I;
void test_statemachine_S11WithISigTransitionToS11(void){
    Event e;
    e.signal = C_SIG;

    stateMachineStart(&state_machine.super);
    stateMachineDispatch(&state_machine.super, &e);

    testStatesInitCounters();

    e.signal = I_SIG;
    stateMachineDispatch(&state_machine.super, &e);

    TEST_ASSERT_EQUAL_PTR(testStates_s11, state_machine.super.state);

    TEST_ASSERT_EACH_EQUAL_INT(0, state_entries, SH_MAX);
    TEST_ASSERT_EACH_EQUAL_INT(0, state_inits, SH_MAX);
    TEST_ASSERT_EACH_EQUAL_INT(0, state_exits, SH_MAX);
    TEST_ASSERT_EACH_EQUAL_INT(0, state_transitions, SH_MAX);
}

#endif // TEST