#include "ADAS_Cfg.h"

#ifndef DEBUG_H
#define DEBUG_H


#ifdef ADAS_DEBUG
#define DPRINT(...)    Serial.print(__VA_ARGS__)
#define DPRINTLN(...)  Serial.println(__VA_ARGS__)
#else
#define DPRINT(...)
#define DPRINTLN(...)
#endif

#ifdef ADAS_DEBUG2
#define DPRINT2(...)    Serial.print(__VA_ARGS__)
#define DPRINTLN2(...)  Serial.println(__VA_ARGS__)
#else
#define DPRINT2(...)
#define DPRINTLN2(...)
#endif


#endif
