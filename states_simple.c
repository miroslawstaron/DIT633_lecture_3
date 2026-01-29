/*
 * states_simple.c
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
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/select.h>
#include <termios.h>

/* Define states */
typedef enum {
    STATE_INIT,
    STATE_STILL,
    STATE_MOVING,
    STATE_STOP
} State;

/* Function needed to check if user pressed exit
    This function reads one character from stdin
    and checks if it is 'x' or 'X'
*/
bool user_pressed_exit(void) {
    char button = getchar();    // read one character at a time from buffer
    
    // check if the button is X
    if (button == 'x' || button == 'X')
        return true;
    return false;
}

/* Non-blocking version of user_pressed_exit
    This function does NOT wait for input, it returns immediately
    Returns true if 'x' or 'X' was pressed
    Puts terminal in raw mode to detect individual key presses
*/
bool user_pressed_exit_nonblocking(void) {
    // Static variables persist across function calls
    // we need it to store terminal settings
    static struct termios old_termios, new_termios;

    // Boolean flag to track if terminal has been configured (only configure once)
    // because this function is called repeatedly in the state machine
    static bool terminal_configured = false;
    
    // Configure terminal to raw mode on first call only
    if (!terminal_configured) {
        // Save the current terminal settings to old_termios 
        // so we can restore them later
        tcgetattr(STDIN_FILENO, &old_termios);

        // Copy the current settings to new_termios to modify them
        new_termios = old_termios;
        
        // Disable canonical mode and echo
        // ICANON: disables canonical (line-buffered) mode 
        // characters available immediately
        // ECHO: disables echoing - characters won't be printed to screen
        new_termios.c_lflag &= ~(ICANON | ECHO);

        // VMIN = 0: minimum characters to read is 0 
        // (non-blocking if no data available)
        new_termios.c_cc[VMIN] = 0;

        // VTIME = 0: timeout in deciseconds 
        // 0 means no timeout, read returns immediately
        new_termios.c_cc[VTIME] = 0;
        
        // Apply the new terminal settings immediately (TCSANOW means "now")
        tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);

        // Mark that terminal has been configured so we don't do it again
        terminal_configured = true;
    }
    
    // Declare a buffer to store the character read from stdin
    char button;
    
    // Read one character from standard input (returns immediately if none available)
    // read() returns the number of bytes read (1 if successful, 0 if no data)
    if (read(STDIN_FILENO, &button, 1) > 0) {
        // Check if the character is 'x' (lowercase) or 'X' (uppercase) for exit command
        if (button == 'x' || button == 'X')
            // User pressed exit key - return true to signal exit
            return true;
    }
    
    // No exit key was pressed - return false to continue
    return false;
}

/* Events / conditions 
    This code is executed only when we enter the condition, not all the time
*/
bool start_moving(void) {
    return true;   
}

bool stop_moving(void) {
    return false;  
}

bool shutdown(void) {
    // check if the user presses a button to exit (x)
    /*
    if (user_pressed_exit()) {
        printf("User requested exit.\n");
        return true;
    }
    */

    // non-blocking version
    if (user_pressed_exit_nonblocking()) {
        printf("User requested exit.\n");
        return true;
    }
    return false;  }

/* State machine implementation 
   @param current_state: the current state of the machine 
   */
State state_machine_step(State current_state) {
    // to implement a statemachine, a switch-case is often used

    // the switch statement is really cool, because it is read from top to bottom
    // it is also very tricky, in the sense that if you forget a "break" statement,
    // the code will "fall through" to the next case!
    switch (current_state) {

        case STATE_INIT:    // one case per state
            printf("State: INIT\n");

            // the return statement steers where we go next in the state machine
            return STATE_STILL;   

        case STATE_STILL:
            printf("State: STILL\n");
            
            // note that in this state, we use the pre-condition function
            // they are only executed once when entering the state
            // but, they are executed EVERY time we enter the state

            // here, we also steer which state we go to next
            // in this case, if start_moving() is set to true, we go to MOVING
            if (start_moving()) 
                return STATE_MOVING;

            // otherwise we stay in STILL
            // please note that "staying" means that we re-enter the state
            // and execute any pre-condition functions again
            // we do not just stay
            return STATE_STILL;

        case STATE_MOVING:
            printf("State: MOVING\n");

            // here, we have three possible transitions
            // we can stop
            if (shutdown()) 
                return STATE_STOP;
            // or we can stop moving
            if (stop_moving()) 
                return STATE_STILL;

            // otherwise, we keep moving
            return STATE_MOVING;

        case STATE_STOP:
            printf("State: STOP\n");
            return STATE_STOP;  // terminal state

        default:
            return STATE_STOP;
    }
}


/* Main for the state machine demo */
int main_states(void) {
    State state = STATE_INIT;

    // this is not the heart of the state machine
    // it is just a loop to keep the state machine running
    // It runs until the state is STOP
    while (state != STATE_STOP) {
        state = state_machine_step(state);
    }

    printf("State machine terminated.\n");

    return 0;
}

/* If we have time during the lecture, I can ask students to add explicit transitions based on
    user input to start and stop moving the state machine.
*/
