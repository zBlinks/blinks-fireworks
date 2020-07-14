/*
 * fireworks.ino
 *
 * Created: 7/11/2020
 *  Author: Mohamed
 */ 

#include "fx/Common.h"
#include "fx/Comm.h"
#include "fx/Dbg.h"
#include "fx/Throttler.h"
#include "StateMachine.h"

// TODO: YUK! move this out later!
StateMngr StateMachine::stateMngr;
byte StateMachine::neighbors = MAX_BYTE;
Timer StateMachine::settle;
StateMachine sm;

// throttle logging to some reasonable rate (used for facevalues dashboard)
DBG_Throttle_With(200);

void setup() {
    // clear log window and show nice label
    DBG_Init();
    DBG_Log("\x1b[2J");
    DBG_LogLn("!#Fireworks!\n");
}

void loop() {
    // This coded will show realtime value on faces
    DBG_Throttle(
        DBG_FaceValues();
    )

    // setup return false until we are ready
    if(!sm.setup()) {
        return;
    }

    // give the state machine a chance to do its thing!
    sm.onTick();

    // update display
    updateDisplay();
}

void updateDisplay() {
    byte s = sm.getState();
    if (s == Triggered) {
        setColor(CYAN);
    }
}

