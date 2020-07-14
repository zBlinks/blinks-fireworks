/*
 * StateMngr.h
 *
 * Manage state transition. Support delayed transition allowing you to slowdownactivity (for debugging or if your
 * scenario requires it)
 * 
 * Transion will always trigger an action, you provide lambda implementing this - StateMachine class takes care of this
 * 
 * This class is not aware of specific state transition rules, but it is still work in progress!
 * 
 * Created: 7/11/2020
 *  Author: Mohamed
 */ 

#ifndef __STATE_MANAGER__
#define __STATE_MANAGER__

enum State;

typedef void (*OnStateChangeFn)(State);

class StateMngr {
private:
  State state, nextState, previousState;
  OnStateChangeFn onStateChange = nullptr;
  Timer timer;
  unsigned char dirty;

public:
  StateMngr() {
    reset();
  }

  void reset() {
    state = Idle;
    previousState = Unknown;
    dirty = false;
  }

  // return current state. Returns Pending if we are in middle of delayed transition
  inline State getState() { return state; }

  // If we are in Pending state, will return most recent state before Pending.
  // otherwise it will return previous state
  inline State getPreviousState() { return previousState; }

  inline void delayedTrigger(State targetState) {
    trigger(targetState, STATE_TRANSISION_DELAY_MS);
  }

  void trigger(State targetState, uint32_t delay = 0) {
    previousState = state;
    nextState = targetState;
    if(delay == 0) {
      immediateTrigger();
    } else {
      delayedTrigger(delay);
    }
  }

  void onStateChanged(void (*osc)(State)) {
    onStateChange = osc;
  }

  void onTick() {
    if(dirty && timer.isExpired()) {
      updateState();
      dirty = false;
    }
  }

private:
  void fireOnStateChange() {
    if(onStateChange != nullptr) {
      onStateChange(state);
    }
  }

  void delayedTrigger(uint32_t ms) {
    state = Pending;
    dirty = true;
    timer.set(ms);
  }

  void immediateTrigger() {
    updateState();
  }

  void updateState() {
      state = nextState;
      fireOnStateChange();
  }
};

#endif // __STATE_MANAGER__