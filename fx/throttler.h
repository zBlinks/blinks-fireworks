/*
 * Throttler.h
 * 
 * A rate limiter essentially. Allow you to execute an action at certain rate.
 *
 * Created: 7/11/2020
 *  Author: Mohamed
 */ 

#ifndef __THROTTLER__
#define __THROTTLER__

class Throttler {
private:
    Timer timer;
    uint16_t milliseconds;

public:
    Throttler (uint16_t millisecondsPeriod) {
        milliseconds = millisecondsPeriod;
    }

    void throttle(void (*action)()) {
        if(timer.isExpired()) {
            action();
            timer.set(milliseconds);
        }
    }
};

#endif  // __THROTTLER__
