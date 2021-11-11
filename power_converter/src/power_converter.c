#include <xttcps.h>
#include <zynq_registers.h>
#include <xuartps_hw.h>
#include <xscugic.h>
#include <xil_exception.h>

#include <xparameters.h>
#include <xstatus.h>
#include <xil_io.h>
#include <xil_types.h>

#include "power_converter.h"
#include "settings_ui.h"

#include "tasks.h"

// PID controller ki and kp
static unsigned int kp = 0; // KP to decrease the rise time
static unsigned int ki = 0; // KI to eliminate the steady-state error

//////////////////////

#define TTC_TICK_DEVICE_ID	XPAR_XTTCPS_0_DEVICE_ID // PS7_TTC_0
#define TTC_TICK_INTR_ID	XPAR_XTTCPS_0_INTR
#define INTC_DEVICE_ID		XPAR_SCUGIC_SINGLE_DEVICE_ID

#define PRESCALEVAL 5
#define TMR_LOADVAL 8000


// Test tasks /////////////

int run_task1(void) {
	// read button status, leds, etc...
	xil_printf("task1\r\n");
	task[0].TaskExecuted = TRUE;
	return 0;
}

int run_task2(void) {
	// do something...
	xil_printf("task2\r\n");
	task[1].TaskExecuted = TRUE;
	return 0;
}

int run_task3(void) {
	// do something...
	xil_printf("task3\r\n");
	task[2].TaskExecuted = TRUE;
	return 0;
}

int run_task4(void) {
	// do something...
	xil_printf("task4\r\n");
	task[3].TaskExecuted = TRUE;
	return 0;
}

//////////////////////

int main(void) {

	// initialize settings user interface
	init_settings_ui();

	// set all buttons to input state
	AXI_BTN_TRI |= 0xF;

	init_interrupts(&InterruptControllerInstance);
	init_timer();
	init_ticker();

	/////////////////////////////////////

	// Add dynamically tasks to task queue
	add_task(2, 4, (void *) run_task1);
	add_task(50, 3, (void *) run_task2);
	add_task(100, 2, (void *) run_task3);
	add_task(200, 3, (void *) run_task4);

	////////////////////////////////////7

	// Main loop
	for (;;) {

		for (uint8_t i = 0; i < get_taskcount(); i++) {

			if (task[i].runtask) {
				task[i].runtask = 0;

				if (task[i].taskrunning) {
					tasktimeout = TRUE;
				}

				task[i].fptr(NULL); // run task function (run_task1(), run_task2()... )
				task[i].TaskExecuted = FALSE;
				// the following must be atomic
				task[i].taskrunning = 0;
			}
		}

	}

	return 0;
}

void init_interrupts(XScuGic *IntcInstancePtr) {

	Xil_ExceptionDisable();
	XScuGic_Config InstanssiConfig;
	XScuGic_Config * IntcConfig = &InstanssiConfig;
	// Initialize the interrupt controller driver
	IntcConfig->DeviceId = XPAR_PS7_SCUGIC_0_DEVICE_ID;	// device driver (SCU) definition
	IntcConfig->CpuBaseAddress = XPAR_PS7_SCUGIC_0_BASEADDR;//CPU interface control register ICCICR
	IntcConfig->DistBaseAddress = XPAR_PS7_SCUGIC_0_DIST_BASEADDR; // Distributor control register ICDDCR
	IntcConfig->HandlerTable[INTC_DEVICE_ID].CallBackRef = NULL;
	IntcConfig->HandlerTable[INTC_DEVICE_ID].Handler = NULL;

	XScuGic_CfgInitialize(IntcInstancePtr, IntcConfig,
			IntcConfig->CpuBaseAddress);

	// Connect the interrupt controller interrupt handler to the hardware interrupt handling logic in the ARM processor.
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT,
			(Xil_ExceptionHandler) XScuGic_InterruptHandler, IntcInstancePtr);

	// Enable interrupts in the ARM
	Xil_ExceptionEnable();

}

void init_timer() {
	TTC0_CNT_CNTRL |= XTTCPS_CNT_CNTRL_DIS_MASK;
	TTC0_CNT_CNTRL = XTTCPS_CNT_CNTRL_RESET_VALUE;

	TTC0_CLK_CNTRL = 0;
	TTC0_INTERVAL_VAL = 0;
	TTC0_MATCH_1 = 0;
	TTC0_MATCH_2_COUNTER_2 = 0;
	TTC0_MATCH_3_COUNTER_2 = 0;
	TTC0_IER = 0;
	TTC0_ISR = XTTCPS_IXR_ALL_MASK;

	TTC0_CNT_CNTRL |= XTTCPS_CNT_CNTRL_RST_MASK;

	TTC0_CNT_CNTRL = XTTCPS_CNT_CNTRL_DIS_MASK | XTTCPS_CNT_CNTRL_INT_MASK;

	TTC0_INTERVAL_VAL = TMR_LOADVAL;
	TTC0_CLK_CNTRL &= ~(XTTCPS_CLK_CNTRL_PS_VAL_MASK
			| XTTCPS_CLK_CNTRL_PS_EN_MASK);
	TTC0_CLK_CNTRL |= (PRESCALEVAL << XTTCPS_CLK_CNTRL_PS_VAL_SHIFT)
			| XTTCPS_CLK_CNTRL_PS_EN_MASK;

	TTC0_CNT_CNTRL2 |= XTTCPS_CNT_CNTRL_DIS_MASK;

	TTC0_CLK_CNTRL2 = (0 << XTTCPS_CLK_CNTRL_PS_VAL_SHIFT)
			| XTTCPS_CLK_CNTRL_PS_EN_MASK;

	TTC0_CNT_CNTRL2 = XTTCPS_CNT_CNTRL_RST_MASK | XTTCPS_CNT_CNTRL_DIS_MASK |
	XTTCPS_CNT_CNTRL_MATCH_MASK | XTTCPS_CNT_CNTRL_POL_WAVE_MASK;

	TTC0_CNT_CNTRL2 &= ~XTTCPS_CNT_CNTRL_DIS_MASK;
}

void init_ticker(void) {
	InterruptControllerInstance.Config->HandlerTable[TTC_TICK_INTR_ID].Handler =
			(Xil_InterruptHandler) tick_handler;
	ICDISER1 = 1 << (TTC_TICK_INTR_ID % 32);

	TTC0_IER |= XTTCPS_IXR_INTERVAL_MASK;
	TTC0_CNT_CNTRL &= ~XTTCPS_CNT_CNTRL_DIS_MASK;
}

// timer interrupt handler, task scheduler
static void tick_handler(void) {

	static unsigned int timer_ticks; // local visibility,as global otherwise, alternative simple tick

	TTC0_ISR; // Cleared on read

	//DATA_2 ^= 0b11<<8;

	// should be in task.c for modularity but performance comes first
	// inlining of course possible

	for (uint8_t i = 0; i < get_taskcount(); i++) {

		if (!task[i].timcount)   		// should we run this task[i]
		{
			if (task[i].taskrunning)   		// if still running
				tasktimeout = 1;				// timeout, not schedulable
			task[i].timcount = task[i].timval; // initialize to total waiting time
			if (!task[i].disabled)
				task[i].runtask = 1; // tell that this should be executed
		} else
			task[i].timcount--;  		// decrement waiting time
	}

	// Sometimes that way not used now, alternative simple tick
	timer_ticks++;
	if (timer_ticks >= 3) {
		timer_ticks = 0;
	}

}

// call PI(D) handler every x ms
static void pi_controller_isr_handler(void) {

	//TTC0_ISR; // Cleared on read

	// PI controller
	/*****************************
	 static float u1_old = 0;
	 float error_new, u1_new, u_new;
	 error_new = y_ref - y_act;
	 u1_new = u1_old + ki * error_new;
	 if (abs(u1_new) >= u1_max) {
	 // no Saturation
	 u1_new = u1_old;
	 }
	 // saturarate integr
	 u_new = kp * error_new + u1_new;

	 u1_old = u1_new;
	 return u_new;

	 setStatusLedColor(u_new);
	 *********************************/
}
