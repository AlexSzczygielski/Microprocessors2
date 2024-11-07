/*----------------------------------------------------------------------------
 *      
 *----------------------------------------------------------------------------
 *      Name:    LEDS.C
 *      Purpose: Microprocessors Laboratory
 *----------------------------------------------------------------------------
 *      
 *      Author: Pawel Russek AGH University of Science and Technology
 *---------------------------------------------------------------------------*/

#include "MKL05Z4.h"                    //Device header
#include "leds.h"												//Declarations

//
// Finite State Machine (FSM) stuff
typedef enum {
	StateA,						// FSM state Red on, Green on, Blue off
  StateB,						// FSM state Red on, Green on, Blue off
  StateC						// FSM state Red on, Green on, Blue on
} FSMStateType;
static FSMStateType fsmState;										//FSM current state

void nextFSMState(void);
unsigned char stateA(void);
unsigned char stateB(void);
unsigned char stateC(void);

//
// Finite State Machine control variables
static const unsigned int FSM_DELAY_MS = 400;	//FSM delay
static unsigned char fastFSM = 1;						  //FSM speed control flag. Initialize to 1: fast
static unsigned char startFSM;							  //FSM run control

static unsigned char keepState = 0;					  //Auxiliary variable to allow FSM slow mode		
static unsigned int nextStateDelayCount;			//FSM delay counter

//
// LED control
typedef enum{ RED = 8, GREEN = 9, BLUE = 10 } LedType; // Define LED position in Port B 
typedef enum{ ON = 1, OFF = 0 } LedStateType; //
void ledCntrl (LedType led, LedStateType state);

//
// Aux. 
static void led_delay(){
volatile int delay;
	for(delay=0; delay<1200000; delay++);
}


/*----------------------------------------------------------------------------
  Function that initializes LEDs
 *----------------------------------------------------------------------------*/
void ledsInitialize(void) {
	
//Initialize registers	
  SIM->SCGC5 |=  SIM_SCGC5_PORTB_MASK;      /* Enable Clock to Port B */ 
  PORTB->PCR[RED] = PORT_PCR_MUX(1);        /* Pin PTB8 is GPIO */
  PORTB->PCR[GREEN] = PORT_PCR_MUX(1);      /* Pin PTB9 is GPIO */
  PORTB->PCR[BLUE] = PORT_PCR_MUX(1);       /* Pin PTE10 is GPIO */
  
	FPTB->PDDR = (1 << RED) | (1 << GREEN) | (1 << BLUE);          /* enable PTB{8, 9, 10} as Output */
	
// init FSM vars.	
	nextStateDelayCount = 0;
	fsmState = StateA;
	startFSM = 1;

//Welcome sequence
	led_delay();
	ledCntrl(RED, ON); ledCntrl(GREEN, ON); ledCntrl(BLUE, ON);
	led_delay();
	ledCntrl(RED, OFF); ledCntrl(GREEN, OFF); ledCntrl(BLUE, OFF);
	led_delay();
	ledCntrl(RED, ON); ledCntrl(GREEN, ON); ledCntrl(BLUE, ON);
	led_delay();
	ledCntrl(RED, OFF); ledCntrl(GREEN, OFF); ledCntrl(BLUE, OFF);
	
}


/*----------------------------------------------------------------------------
  Function that controls selected LED
  led = {RED, GREEN, BLUE}
 *----------------------------------------------------------------------------*/
void ledCntrl (LedType led, LedStateType state) {
	if( state == ON)
		FPTB->PCOR |= (1 << led);
	if( state == OFF)
		FPTB->PSOR |= (1 << led);
}


/*----------------------------------------------------------------------------
 Function that moves FSM to phase A 
 *----------------------------------------------------------------------------*/
FSMStateType stateA(void){
	ledCntrl(RED, ON);
	ledCntrl(GREEN, OFF);
	ledCntrl(BLUE, OFF);
	return StateB;								/*Return next state of FSM*/
}

/*----------------------------------------------------------------------------
 Function that moves FSM to phase B   
 *----------------------------------------------------------------------------*/
FSMStateType stateB(void){
	ledCntrl(RED, OFF);
	ledCntrl(GREEN, ON);
	ledCntrl(BLUE, OFF);
	return StateC;								/*Return next state of FSM*/				
}

/*----------------------------------------------------------------------------
 Function that moves FSM to phase C     
 *----------------------------------------------------------------------------*/
FSMStateType stateC(void){
	ledCntrl(RED, OFF);
	ledCntrl(GREEN, OFF);
	ledCntrl(BLUE, ON);
	return StateA;								/*Return next state of FSM*/
}

/*----------------------------------------------------------------------------
 Function that makes FSM transition to the next state  
 *----------------------------------------------------------------------------*/
void nextFSMState (void) {	
 
	/** Control speed of the FSM */
	if ( fastFSM==0 ) {
			keepState += 1;
			if( (keepState%3) != 0 ) return;		/* Every third call passes  to switch clause*/
	}
	
	switch(fsmState){
		case StateA:
			fsmState = stateA();
			break;
		case StateB:
			fsmState = stateB();
		  break;
		default:
			fsmState = stateC();
		  break;
	}
}

/********************* Functions accessed globally*****************************/

/*----------------------------------------------------------------------------
 Function controls transition speed of the FSM  
 *----------------------------------------------------------------------------*/
void fsmFastSlow(void){
	fastFSM^=1;				/* Toggle speed mode */
}

/*----------------------------------------------------------------------------
 Function that stops FSM transitions     
 *----------------------------------------------------------------------------*/
void fsmStartStop(void){
	startFSM^=1;
}

/*----------------------------------------------------------------------------
 Every 1 ms function that makes FSM run    
 *----------------------------------------------------------------------------*/
void fsmService (void) {	
 
	if( nextStateDelayCount!= FSM_DELAY_MS ){
		nextStateDelayCount += startFSM;
		return;
	}
	
	nextStateDelayCount = 0;
	nextFSMState();
	
}









