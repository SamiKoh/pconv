#ifndef POWER_CONVERTER_H
#define POWER_CONVERTER_H


XScuGic InterruptControllerInstance; // Interrupt controller instance

void init_timer();
void init_ticker(void);

void init_interrupts(XScuGic *IntcInstancePtr);
void init_timer();

int run_task1(void);
int run_task2(void);
int run_task3(void);
int run_task4(void);

static void tick_handler(void);
static void pi_controller_isr_handler(void); // Interrupt handler

#endif
