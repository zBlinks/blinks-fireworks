/*
 * StateMachine.h
 *
 * Encapsulates state transition triggers and actions
 * 
 * Created: 7/11/2020
 *  Author: Mohamed
 */ 

#ifndef __STATE_MACHINE__
#define __STATE_MACHINE__

#include "States.h"
#include "fx/StateMngr.h"

class StateMachine
{
private:
    static StateMngr stateMngr;
    static byte neighbors;
    static Timer settle;

public:
    StateMachine() {
        settle.set(500);
        stateMngr.onStateChanged([](State s) {
            DBG_LogL("State: ", s);
            switch (s) {
                //══════════════════╦══════════════════════════════════════════════════════════════════════════════════════════
                case Waiting://     ║
                //══════════════════╝
                    Comm::broadcast(CMD_SPREAD);
                    break;

                //══════════════════╦══════════════════════════════════════════════════════════════════════════════════════════
                case Triggered://   ║
                //══════════════════╝
                {
                    if (neighbors == 6) {
                        // I'm a star! trigger all neigbors
                        Comm::broadcast(CMD_TRIGGER);
                    } else {
                        // I'm no star! if a star is my neigbor, trigger it otherwise trigger all
                        byte max = Comm::maxValue();
                        if (max == 6) {
                            Comm::sendToFacesWith(max, CMD_TRIGGER);
                        } else {
                            Comm::broadcast(CMD_TRIGGER);
                        }
                    }
                }
                break;

                //══════════════════╦══════════════════════════════════════════════════════════════════════════════════════════
                case Reset://       ║
                //══════════════════╝
                    // trigger other blinks reset neighbors
                    Comm::broadcast(CMD_RESET_ENTER);
                    break;
            }
        });
    }

    inline State getState() { return stateMngr.getState(); }

    /*
     * return true if ready, otherwise false. false mean don't call tick() just yet
     * loop will continue to call setup its your responsibility to be idempotnet.
     */
    bool setup() {
        // I tried broadcasting # of neigbors on arduino setup but doesn't seem to work there, so
        // had to use this code to introduce some delaye

        // we are ready when we know # of neigbors we have
        if(neighbors != MAX_BYTE) {
            return true;
        }

        // wait sometime for things to settle  before calculating # neigbors
        if (settle.isExpired()) {
            // Initialize neigbors count
            neighbors = Comm::countNeighbors();
            Comm::broadcast(PACK_VALUE(neighbors));
            return true;
        }

        return false;
    }

    void onTick() {
        auto s = stateMngr.getState();
        runEvaluationTask(s);
        stateMngr.onTick();
    }

private:
    void runEvaluationTask(State s) {
        // Handle button
        if (buttonSingleClicked() && !hasWoken()) {
            stateMngr.delayedTrigger(Triggered);
        }

        if (buttonDoubleClicked()) {
            if(s == Triggered) {
                stateMngr.trigger(Reset);
            } else if (s == Reset) {
                reset();
            }
        }

        // Handle comm
        switch (s) {
            //══════════════════╦══════════════════════════════════════════════════════════════════════════════════════
            case Idle://        ║
            //══════════════════╝
                if (Comm::anyNeighborSent(CMD_TRIGGER)) {
                    if (neighbors == 6) {
                        stateMngr.trigger(Waiting);
                    } else {
                        stateMngr.delayedTrigger(Triggered);
                    }
                } else if (Comm::anyNeighborSent(CMD_SPREAD)) {
                    byte src = Comm::whichNeighborSent(CMD_SPREAD);
                    setValueSentOnFace(0, Comm::faceBefore(src));
                    setValueSentOnFace(0, Comm::faceAfter(src));
                    setValueSentOnFace(CMD_READY, src);
                }
                break;

            //══════════════════╦══════════════════════════════════════════════════════════════════════════════════════
            case Triggered://   ║
            //══════════════════╝
                if (Comm::anyNeighborSent(CMD_RESET_ENTER)) {
                    stateMngr.trigger(Reset);
                } else if (Comm::anyNeighborSent(CMD_SPREAD)) {
                    // a stat can send to spread to the neigbor triggered it. This neighbor is already triggered
                    // and should reply to star with CMD_READY as star will not spread the flame  untill all
                    // neighbors reported ready.                
                    byte src = Comm::whichNeighborSent(CMD_SPREAD);
                    setValueSentOnFace(CMD_READY, src);
                }
                break;

            //══════════════════╦══════════════════════════════════════════════════════════════════════════════════════
            case Waiting://     ║
            //══════════════════╝
                if (Comm::allNeighborsSent(CMD_READY)) {
                    stateMngr.trigger(Triggered);
                }
                break;

            //══════════════════╦══════════════════════════════════════════════════════════════════════════════════════
            case Reset://       ║
            //══════════════════╝
                if (Comm::anyNeighborSent(CMD_RESET_COMPLETE)) {
                    reset();
                }
                break;
        }
    }

    void reset() {
        neighbors = MAX_BYTE;
        settle.set(500);
        stateMngr.reset();
        setColor(OFF);
        Comm::broadcast(CMD_RESET_COMPLETE);
    }
};

#endif // __STATE_MACHINE__