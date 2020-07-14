/*
 * states.h
 *
 * Created: 7/11/2020
 *  Author: Mohamed
 */ 

#ifndef __STATES__
#define __STATES__

/*
 * Distinction between State and Command is merely semantics at this point but may be useful later
 */
#define CMD_TRIGGER 7
#define CMD_SPREAD  8
#define CMD_READY   9
#define CMD_RESET_ENTER   10
#define CMD_RESET_COMPLETE  11

// This is default transition delay if not passed to trigger function
#define STATE_TRANSISION_DELAY_MS 25

enum State {
  //───────────────────────────────────────────────────────────────────────────────────────────────
  //                                         Reserved States 
  //───────────────────────────────────────────────────────────────────────────────────────────────
  Unknown,    // zero is the default value on face, do not using it
  Pending,    // Pending reported when we are in the middle of delayed transition
  Reset,      // Resest com values seen on faces (all will be set to Reset). This way we can reset
              // our state machine without getting things missed up due to resedual values on faces
  
  //───────────────────────────────────────────────────────────────────────────────────────────────
  //                                       User defined states
  //───────────────────────────────────────────────────────────────────────────────────────────────
  Idle,       // We just started, we'll wait for button press
  Waiting,    // A star waiting for its neighbors to get ready
  Triggered   // I'm on
};

#endif  // __STATES__