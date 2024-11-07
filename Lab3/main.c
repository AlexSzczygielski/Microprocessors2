/*----------------------------------------------------------------------------
 *      Main: Initialize
 *---------------------------------------------------------------------------*/

#include "MKL05Z4.h"                    	/* Device header */
#include "fsm.h"													/* leds functions for FRDM46-KL46 */
#include "pit.h"
#include "buttons.h"											/* Buttons of external keyboard */

/*----------------------------------------------------------------------------
  SysTick_Handler  function is an interrupt service routine 
	for ARM core Sys_Tick interrupt
 *----------------------------------------------------------------------------*/
void runThisISREvery1ms(void) { 							/* ToDo 4.1:  Put the name of SysTick handler */	
	fsmOneMSService();												
}


int main(void) {
int n;	
	fsmInitialize();       	/* Initialize LEDs */
	//buttonsInitialize();	  /* ToDo 5.7 Initialize buttons */
		
	
	//SysTick_Config(SystemCoreClock / <trim for 1ms>);	/** ToDo 4.2: Configure and anable SysTick interrupt to generate an interrupt every one millisecond */
																						          /* The function initializes the System Timer and its interrupt, and starts the System Tick Timer. */
																						          /* Counter is in free running mode to generate periodic interrupts. */
																						          /* Function parameter is a umber of system ticks between two interrupts */
																											/** ToDo 6.7: comment when PIT is enabled */
	
	//pitInitialize( <LDVAL value for 1 ms period> );   /* ToDo 6.5 Initilaize PIT and set period value for 1 ms */
	// startPIT(); /** ToDo 6.6. Uncomment */
	
		
	while(1){	
	 __WFI(); // Save energy and wait for interrupt
	}
	
	
}

