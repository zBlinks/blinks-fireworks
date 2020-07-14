/*
 * Comm.h
 * 
 * Useful communication utility
 *
 * Created: 7/11/2020
 *  Author: Mohamed
 */ 

#ifndef __COMM_H__
#define __COMM_H__

class Comm {
public:

    /*
     * Zero out all faces
     */
    static void reset() { setValueSentOnAllFaces(0); }
    static byte maxValue() {
        int8_t max = -1;
        FOREACH_FACE(f) {
            if (!isValueReceivedOnFaceExpired(f)) {
                byte value = getLastValueReceivedOnFace(f);
                if(CHECK_VALUE(value)) {    // ignore commands
                    value = UNPACK_VALUE(value);
                    if (value > max) {
                        max = value;
                    }
                }
            }
        }

        return max;
    }

    /*
     * Get the value of face right bofore the one passed
     */
    static byte faceBefore(byte face) {
        return face == 0 ? 5 : face -1;
    }

    /*
     * Get value of face right after the one passed
     */
    static byte faceAfter(byte face) {
        return face == 5 ? 0 : face + 1;
    }

    /*
     * Send the given value to the face with the value matched the marker. Reset all other faces.
     * Marker is a value v such that v & 0x11000 is true. This is really arbitrary!
     */
    static void sendToFacesWith(byte marker, byte value) {
        FOREACH_FACE(f) {
            if (!isValueReceivedOnFaceExpired(f)) {
                byte b = getLastValueReceivedOnFace(f);
                if(CHECK_VALUE(b)) {
                    b = UNPACK_VALUE(b);
                    if (b == marker) {
                        setValueSentOnFace(value, f);
                    } else {
                        setValueSentOnFace(0, f);
                    }
                }
            }
        }
    }

    /*
     * count neighbors sent the given value. Passing -1 will count all neighbors
     */
    static byte countNeighborsSent(byte value) {
        byte count = 0;
        FOREACH_FACE(f) {
            if (!isValueReceivedOnFaceExpired(f)) {
                if(value == MAX_BYTE || getLastValueReceivedOnFace(f) == value) {
                    count++;
                }
            }
        }

        return count;
    }

    /*
     * Returns the index of neighbor sent max value to us. -1 if no neighbor.
     * Recall, value is distinguished from command by using mask 0x110000
     */
    static byte neighborSentMaxResponse() {
        byte max = 0;
        byte face = MAX_BYTE;
        FOREACH_FACE(f) {
            if (!isValueReceivedOnFaceExpired(f)) {
                byte value = getLastValueReceivedOnFace(f);
                if(CHECK_VALUE(value)) {
                    byte resp = UNPACK_VALUE(value);
                    if(resp >= max) {
                        max = resp;
                        face = f;
                    }
                }
            }
        }

        return face;
    }

    /*
     * Handy to check if we received a value from any neighbor
     */
    static bool anyNeighborSent(byte value) {
        return Comm::countNeighborsSent(value) > 0;
    }

    /*
     * Handy to check if all neighbors sent us a specific value
     */
    static bool allNeighborsSent(byte value) {
        byte hasNeighbors = false;
        FOREACH_FACE(f) {
            if (!isValueReceivedOnFaceExpired(f)) {
                hasNeighbors = true;
                if(getLastValueReceivedOnFace(f) != value) {
                    return false;
                }
            }
        }

        return hasNeighbors;
    }

    /*
     * Return the index of the neighbor sent the given value
     */
    static byte whichNeighborSent(byte b) {
        FOREACH_FACE(f) {
            if (!isValueReceivedOnFaceExpired(f)) {
                if(getLastValueReceivedOnFace(f) == b) {
                    return f;
                }
            }
        }

        return MAX_BYTE;
    }

    /*
     * return numbers of neighbors we have
     */
    inline static byte countNeighbors() {
        return Comm::countNeighborsSent(MAX_BYTE);
    }

    /*
     * broadcast a value to all neighbors
     */
    inline static void broadcast(byte value) {
        setValueSentOnAllFaces(value);
    }
};

#endif  // __COMM_H__