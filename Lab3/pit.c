/*----------------------------------------------------------------------------
 *      Name:    pit.c
 *
 *      Purpose: Microprocessors Laboratory 2
 *      
 *      Author: Pawel Russek AGH University of Science and Technology
 *---------------------------------------------------------------------------*/
 
#include "MKL05Z4.h"                    //Device header
#include "pit.h"												//Declarations
#include "fsm.h"

static const IRQn_Type myPIT_IRQn = -1;  /*-1 is not a correct value */	/* ToDo 6.1: Define interrupt (IRQ) number for PIT. Check MKL05Z4.h or find in NVIC section of KL05 Sub-Family Reference Manual - NVIC section */ 


void PIT_IRQHandler() {
		
	// check to see which channel triggered interrupt 
	if (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) {
		// clear status flag for timer channel 0
		PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;
		// Do ISR work
		fsmOneMSService();
	} else if (PIT->CHANNEL[1].TFLG & PIT_TFLG_TIF_MASK) {
		// clear status flag for timer channel 1
		PIT->CHANNEL[1].TFLG &= PIT_TFLG_TIF_MASK;
	}

	//clear pending IRQ
	NVIC_ClearPendingIRQ(myPIT_IRQn);
	
}

 /*----------------------------------------------------------------------------
  Function that initializes PIT timer
 *----------------------------------------------------------------------------*/
void pitInitialize(unsigned period) {

	// Enable clock to PIT module
	// SIM->SCGC6 |= SIM_SCGC6_XXX_MASK; /* ToDo 6.2 Enable PIT mask in SCGC6 register */
	
	// Enable module, freeze timers in debug mode
	//PIT->MCR &= ~PIT_MCR_XXX_MASK; /* ToDo 6.3 Disable MDIS mask in MCR register */
	//PIT->MCR |= PIT_MCR_XXX_MASK;   /* ToDo 6.4 Enable FRZ mask in MCR register */
	
	// Initialize PIT0 to count down from argument 
	PIT->CHANNEL[0].LDVAL = PIT_LDVAL_TSV(period);

	// No chaining
	PIT->CHANNEL[0].TCTRL &= PIT_TCTRL_CHN_MASK;
	
	// Generate interrupts
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;

	/* Enable Interrupts */
	//NVIC_SetPriority(myPIT_IRQn, 2); // 0, 1, 2 or 3 /* ToDo 7.2b, ToDo 7.3: Set PIT interrupt priority level  */

	NVIC_ClearPendingIRQ(myPIT_IRQn); 
	NVIC_EnableIRQ(myPIT_IRQn);	

}


void startPIT(void) {
// Enable counter
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
}

void stopPIT(void) {
// Enable counter
	PIT->CHANNEL[0].TCTRL &= ~PIT_TCTRL_TEN_MASK;
}


