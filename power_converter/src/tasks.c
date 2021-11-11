#include <stdio.h>
#include <xil_types.h>

#include "tasks.h"

static uint8_t taskcount = 0;

unsigned char add_task(unsigned short timval, unsigned char taskpri,
		uint32_t *ptask) {
	if (taskcount >= TASKS_MAX)
		return FALSE;

	for (uint8_t i = 0; i < TASKS_MAX; i++) {
		// init task
		if (task[i].fptr == NULL) {
			task[i].timval = timval;
			task[i].taskpri = 4;
			task[i].TaskExecuted = TRUE;

			task[i].timcount = task[i].timval;
			task[i].taskrunning = 0;
			task[i].runtask = 0;
			task[i].disabled = 0;

			task[i].fptr = (void *) ptask; // pointer to task

			taskcount++;

			return TRUE;
		}
	}
	return FALSE;
}

uint8_t get_taskcount() {
	return taskcount;
}

