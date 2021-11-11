#ifndef TASKS_H
#define TASKS_H

#define TASKS_MAX 10

#define TASK1 0
#define TASK2 1
#define TASK3 2
#define TASK4 3

struct tasks {
	unsigned short timcount;
	unsigned short timval;
	unsigned char taskpri;
	unsigned char disabled;
	unsigned char taskrunning;
	unsigned char runtask;
	unsigned char TaskExecuted :1;
	void (*fptr)(void *);
};

volatile struct tasks task[TASKS_MAX];

volatile unsigned char runpri, tasktimeout;

extern uint8_t get_taskcount();

extern unsigned char add_task(unsigned short timval, unsigned char taskpri, uint32_t *ptask);


#endif
