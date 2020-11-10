#ifndef NODE1_FSM_H
#define NODE1_FSM_H


enum FSM_STATE {
    INIT,
    MENU,
    PLAYING,
    POSTGAME
};


enum FSM_STATE fsm_get_state(void);


void fsm_set_state(enum FSM_STATE state);


void fsm_transition_to(enum FSM_STATE state);


#endif