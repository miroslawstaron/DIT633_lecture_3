/*
 * simple_states_transition_table.c
 * 
 * Copyright (c) 2026 Miroslaw Staron
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdbool.h>
#include "state_machine.h"

/* Define states */
typedef enum {
    STATE_INIT,
    STATE_STILL,
    STATE_MOVING,
    STATE_STOP,
    STATE_COUNT
} State;

/* Transition table entry
   - from: state where this transition is considered
   - guard: function returning true when the transition should be taken
   - to: next state if guard is true
*/
typedef struct {
    State from;
    bool (*guard)(void);
    State to;
} Transition;

/* The transition table implements the state diagram:
   [*] -> Init -> Still -> Moving -> Still
                           \-> Stop -> [*]
   We keep order so that shutdown (Moving -> Stop) is checked before stop_moving.
*/
static const Transition transitions[] = {
    
    // format for each row: 
    // from state, guard function, to state
    { STATE_INIT,   NULL,           STATE_STILL },
    { STATE_STILL,  start_moving,   STATE_MOVING },
    { STATE_MOVING, shutdown,       STATE_STOP },
    { STATE_MOVING, stop_moving,    STATE_STILL },
    { STATE_STOP,   NULL,           STATE_STOP }
};

/* Helper: print state name */
static const char *state_name(State s) {
    switch (s) {
        case STATE_INIT:   return "INIT";
        case STATE_STILL:  return "STILL";
        case STATE_MOVING: return "MOVING";
        case STATE_STOP:   return "STOP";
        default:           return "UNKNOWN";
    }
}

/* Table-driven state machine step
   - Look for the first transition whose 'from' matches current_state and whose guard is true (or NULL)
   - If guard == NULL we treat it as unconditional (always true)
   - If no transition matches for the state, re-enter the same state (this keeps behavior similar to your original code)
*/
State state_machine_step_transitions(State current_state) {
    printf("State: %s\n", state_name(current_state));

    // Iterate transitions in order
    // sizeof(transitions)/sizeof(transitions[0]) - number of entries in the table
    for (size_t i = 0; i < sizeof(transitions)/sizeof(transitions[0]); ++i) {

        // Get pointer to current transition
        const Transition *t = &transitions[i];

        // Check if this transition is applicable for the current state
        if (t->from != current_state) continue;

        // Check guard condition
        if (t->guard == NULL) {
            /* unconditional transition */
            return t->to;
        } else {
            /* conditional transition */
            if (t->guard()) {
                return t->to;
            }
        }
    }

    // No transition matched: 
    // re-enter the same state 
    // (execute entry actions again in your model)
    return current_state;
}

/* Main for the state machine demo */
int main_transitions(void) {
    State state = STATE_INIT;

    printf("State machine (table-driven) started.\n");
    printf("Note: user_pressed_exit() reads one character from stdin and returns true on 'x' or 'X'.\n");
    printf("Press 'x' then Enter to request shutdown when in MOVING.\n\n");

    while (state != STATE_STOP) {
        state = state_machine_step_transitions(state);
    }

    printf("State machine terminated.\n");
    return 0;
}
