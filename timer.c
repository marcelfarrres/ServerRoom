// ADT Timer

#include <xc.h>
#include "pic18f4321.h"
#include "timer.h"

//interrupcion cada 1ms if OSC- 40Mhz
#define T0CON_CONFIG 0x80 
#define RELOAD_TMR0 60536        

#define TI_NUMTIMERS 2             // Maximum mumber of virtual timers, to be adjusted at will

// VARIABLES GLOBALS DEL TAD
struct Timer {
	unsigned long InitialTics;
	unsigned char Busy;
} static Timers[TI_NUMTIMERS];

static volatile unsigned long Tics=0;

void RSI_Timer0 () {
    // Pre: IMPORTANT! This function needs to be called from the Interrupt Service Routine (ISR) when TMR0IF==1.
    TMR0=RELOAD_TMR0;
    TMR0IF=0;
    Tics++;    
}

void TI_Init () {
	//Pre: Global interrupts need to be enabled
	for (unsigned char counter=0; counter<TI_NUMTIMERS; counter++) {
		Timers[counter].Busy=TI_FALSE;
	}
	T0CON=T0CON_CONFIG;
        TMR0=RELOAD_TMR0;
	INTCONbits.TMR0IF = 0;
	INTCONbits.TMR0IE = 1;
}

unsigned char TI_NewTimer(unsigned char *TimerHandle) {
	unsigned char Counter=0;
	while (Timers[Counter].Busy==TI_TRUE) {
		if (++Counter == TI_NUMTIMERS) return (TI_FALSE);
	}
	Timers[Counter].Busy=TI_TRUE;
	*TimerHandle=Counter;
    return (TI_TRUE);
}

void TI_ResetTics (unsigned char TimerHandle) {
	di(); Timers[TimerHandle].InitialTics=Tics; ei();
}


unsigned long TI_GetTics (unsigned char TimerHandle) {
    di(); unsigned long CopyCurrentTics=Tics; ei();
	return (CopyCurrentTics-(Timers[TimerHandle].InitialTics));
}

void TI_CloseTimer (unsigned char TimerHandle) {
	Timers[TimerHandle].Busy=TI_FALSE;
}

void TI_End () {
}