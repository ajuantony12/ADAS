/**
* @file ADAS_Debug.h
* @author Christoph Jurczyk
* @date January 29, 2019
* @brief This file contains redefinition of the serial functions for debugging purpose
*
*/

#include "ADAS_Cfg.h"

#ifndef DEBUG_H
#define DEBUG_H

/** Debug enabled: remap DPRINT and DPRINTLN to Serial functions */
#ifdef ADAS_DEBUG

/** To print characters for debugging purpose use DPRINT. 
	For example: @code DPRINT("Hello World...")@endcode */
#define DPRINT(...)    Serial.print(__VA_ARGS__)

/** To print line for debugging purpose use DPRINTLN. 
	For example: @code DPRINTLN("Hello World!")@endcode */
#define DPRINTLN(...)  Serial.println(__VA_ARGS__)

/** Debug disabled: all DPRINT and DPRINTLN are disabled */
#else
#define DPRINT(...)
#define DPRINTLN(...)

#endif


#endif
