#include "statemachine.h"

Event initEvent = { INIT_SIG };
Event entryEvent = { ENTRY_SIG };
Event exitEvent = { EXIT_SIG };
Event emptyEvent = { EMPTY_SIG };
Event* nullEvent = ((Event*)0);

void stateMachineStart(StateMachine* sm){
    StateHandler top_state = sm->state;     // function pointer to top
    StateHandler current_state = sm->temp;  // function pointer to init

    State status = (current_state)(sm, &emptyEvent); // transition to init

    while(status == TRAN_STATUS){
        StateHandler state_list[STATEMACHINE_MAXIMUM_NEST_DEPTH];
        int state_count = 0;
        
        current_state = sm->temp; // current state is state to which the transition points to

        // fill list with current state and super states
        while(current_state != top_state){

            // if maximum depth reached exit and initial states remain untouched
            if(state_count >= STATEMACHINE_MAXIMUM_NEST_DEPTH){
                return;
            }

            state_list[state_count++] = current_state;
            (current_state)(sm, &emptyEvent);
            current_state = sm->temp;
        }

        // execute entry events for all states in list
        while(state_count > 0){
            (state_list[--state_count])(sm, &entryEvent);
        }

        top_state = state_list[0]; // new top state is old current state
        status = (state_list[0])(sm, &initEvent); // send init signal to old current state
    }

    sm->state = top_state;
    sm->temp = top_state;
}

static signed int stateMachineTransition(StateMachine* sm, StateHandler path[STATEMACHINE_MAXIMUM_NEST_DEPTH]){
   signed int ip = -1; /* transition entry path index */
    StateHandler t = path[0];
    StateHandler const s = path[2];

    /* (a) check source==target (transition to self)... */
    if (s == t) {
        (s)(sm, &exitEvent);
        ip = 0; /* enter the target */
    }
    else {
        /* find superstate of target */
        (void)(t)(sm, &emptyEvent);

        t = sm->temp;

        /* (b) check source==target->super... */
        if (s == t) {
            ip = 0; /* enter the target */
        }
        else {
            /* find superstate of src */
            (void)(s)(sm, &emptyEvent);

            /* (c) check source->super==target->super... */
            if (sm->temp == t) {
                (s)(sm, &exitEvent);
                ip = 0; /* enter the target */
            }
            else {
                /* (d) check source->super==target... */
                if (sm->temp == path[0]) {
                    (s)(sm, &exitEvent);
                }
                else {
                    /* (e) check rest of source==target->super->super..
                    * and store the entry path along the way
                    */
                    signed int iq = 0; /* indicate that LCA not found */
                    ip = 1; /* enter target and its superstate */
                    path[1] = t;      /* save the superstate of target */
                    t = sm->temp; /* save source->super */

                    /* find target->super->super... */
                    State r = (path[1])(sm, &emptyEvent);
                    while (r == SUPER_STATUS) {
                        ++ip;
                        path[ip] = sm->temp; /* store the entry path */
                        if (sm->temp == s) { /* is it the source? */
                            iq = 1; /* indicate that LCA found */
                            --ip; /* do not enter the source */
                            r = HANDLED_STATUS; /* terminate loop */
                        }
                         /* it is not the source, keep going up */
                        else {
                            r = (sm->temp)(sm, &emptyEvent);
                        }
                    }

                    /* the LCA not found yet? */
                    if (iq == 0) {
                        (s)(sm, &exitEvent); /* exit the source */

                        /* (f) check the rest of source->super
                        *                  == target->super->super...
                        */
                        iq = ip;
                        r = IGNORED_STATUS; /* LCA NOT found */
                        do {
                            if (t == path[iq]) { /* is this the LCA? */
                                r = HANDLED_STATUS; /* LCA found */
                                ip = iq - 1; /* do not enter LCA */
                                iq = -1; /* cause termintion of the loop */
                            }
                            else {
                                --iq; /* try lower superstate of target */
                            }
                        } while (iq >= 0);

                        /* LCA not found? */
                        if (r != HANDLED_STATUS) {
                            /* (g) check each source->super->...
                            * for each target->super...
                            */
                            r = IGNORED_STATUS; /* keep looping */
                            do {
                                /* exit t unhandled? */
                                if ((t)(sm, &exitEvent) == HANDLED_STATUS){
                                    (void)(t)(sm, &emptyEvent);
                                }
                                t = sm->temp; /* set to super of t */
                                iq = ip;
                                do {
                                    /* is this LCA? */
                                    if (t == path[iq]) {
                                        /* do not enter LCA */
                                        ip = (signed int)(iq - 1);
                                        iq = -1; /* break out of inner loop */
                                        r = HANDLED_STATUS; /* break out of outer loop */
                                    }
                                    else {
                                        --iq;
                                    }
                                } while (iq >= 0);
                            } while (r != HANDLED_STATUS);
                        }
                    }
                }
            }
        }
    }
    return ip;
}

void stateMachineDispatch(StateMachine* sm, Event* e) {
    StateHandler t = sm->state; /* save for later */

    State r;
    StateHandler s;

    do {
        s = sm->temp;
        r = (s)(sm, e); /* invoke state handler s */

        if (r == UNHANDLED_STATUS) { /* unhandled due to a guard? */
            r = (s)(sm, &emptyEvent);
        }

    } while (r == SUPER_STATUS);

    if(r == TRAN_STATUS){
        StateHandler path[STATEMACHINE_MAXIMUM_NEST_DEPTH];
        path[0] = sm->temp;
        path[1] = t;
        path[2] = s;

        for(; t != s; t = sm->temp){

            if((t)(sm, &exitEvent) == HANDLED_STATUS){
                (void)(t)(sm, &emptyEvent);
            }
        }

        signed int ip;

        ip = stateMachineTransition(sm, path);
        
        /* execute state entry actions in the desired order... */
        for (; ip >= 0; --ip) {
            (void)(path[ip])(sm, &entryEvent);
        }

        t = path[0];  /* stick the target into register */
        sm->temp = t; /* update the next state */

        /* while nested initial transition... */
        while ((t)(sm, &initEvent) == TRAN_STATUS) {

            ip = 0;
            path[0] = sm->temp;

            /* find superstate */
            (sm->temp)(sm, &emptyEvent);

            while (sm->temp != t) {
                ++ip;
                path[ip] = sm->temp;
                /* find superstate */
                (sm->temp)(sm, &emptyEvent);
            }
            sm->temp = path[0];

            /* retrace the entry path in reverse (correct) order... */
            do {
                (path[ip])(sm, &entryEvent);
                --ip;
            } while (ip >= 0);

            t = path[0]; /* current state becomes the new source */
        }
    }

    sm->state = t; /* change the current active state */
    sm->temp  = t; /* mark the configuration as stable */


}

void stateMachineInit(StateMachine* sm, StateHandler* init_handler){
    sm->state = (StateHandler)stateMachineTop;
    sm->temp = (StateHandler)init_handler;
}

State stateMachineTop(StateMachine* sm, Event* e){
    return IGNORED_STATUS;
}

State stateMachineError(StateMachine* sm, Event* e){
    return ERROR_STATUS;
}