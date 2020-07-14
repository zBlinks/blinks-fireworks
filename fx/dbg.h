/*
 * Dbg.h
 *
 * Created: 7/11/2020
 *  Author: Mohamed
 */ 

#ifndef __DBG_H__
#define __DBG_H__

#include "Serial.h"
#include "common.h"

/*
 * To Use: Call Init once, the you can call others as many.
 */
#if __DBG_ENABLED__
#define DBG_Init()          Dbg::init()
#define DBG_Log(t)          Dbg::log(t)
#define DBG_LogLn(t)        Dbg::log(t);Dbg::newLine()
#define DBG_LogL(l, t)      Dbg::log(l, t)
#define DBG_Sample(i, v)    Dbg::log(i, v)
#define DBG_FaceValues()    Dbg::faceValues()

#define DBG_Throttle_With(ms)   Throttler throttler(ms)
#define DBG_Throttle(c)         throttler.throttle([]() {   \
                                                        {c} \
                                                        });
#else
#define DBG_Init()
#define DBG_Log(x)
#define DBG_LogLn(t)
#define DBG_LogL(x, y)
#define DBG_Sample(i, v)
#define DBG_FaceValues()

#define DBG_Throttle_With(ms)   Throttler throttler(ms)
#define DBG_Throttle(c)
#endif

#if __DBG_ENABLED__
class Dbg {
private:
    static ServicePortSerial serial;

public:
    static void init() { serial.begin(); }
    static inline void newLine() { serial.println(""); }
    static void log(const char* text) { serial.print(text); }
    static void log(byte b) {
        char psz[] = "0x00";
        byteToString(b, psz);
        serial.print(psz);
    }

    static void log(byte index, byte b) {
        char psz[] = "0x00";
        byteToString(b, psz);
        log(index, psz);
    }
    
    static void log(byte index, const char* text) {
        char byteTmpl[] = "00";
        if(index < 10) {
            byteTmpl[0] = '0' + index;
            byteTmpl[1] = '\0';
        } else {
            byteTmpl[0] = '0' + index / 10;
            byteTmpl[1] = '0' + index % 10;
        }
        
        serial.print("@{");
        serial.print(byteTmpl);
        serial.print(":");
        serial.print(text);
        serial.print("}");
    }

    static void log(const char* label, byte b) {
        serial.print(label);
        Dbg::log(b);
        serial.println("");
    }

    static void faceValues() {
        FOREACH_FACE(f) {
            if (isValueReceivedOnFaceExpired(f)) {
                Dbg::log(f, "-");
            } else {
                byte value = getLastValueReceivedOnFace(f);
                Dbg::log(f, CHECK_VALUE(value) ? UNPACK_VALUE(value) : value);
            }
        }
    }

private:
    static void byteToString(byte b, char* psz) {
        byte n;
        n = b>>4;
	    psz[2] = ( n < 10) ? '0' + n : 'A' + n-10;
	    n = b&0x0F;
	    psz[3] = ( n < 10) ? '0'+n : 'A' + n-10;
    }
};
#endif  // __DBG_ENABLED__

#endif  // __DBG_H__