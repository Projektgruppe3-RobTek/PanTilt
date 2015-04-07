#pragma once
#define MAX_TASKS 255

#if MAX_TASKS > 255
	#error "Can't have more than 255 tasks!"
#endif

#include "../../headers/emp_type.h"
#include <stdbool.h>
typedef enum
{
	DEAD,
	RUNNING,
	BLOCKED,
	WAITING,
} task_status;

typedef struct process
{
	INT8U id;
	//Pointer to the process function.
	void (*process_func)(void);
	//Pointer for eventual check function.
	//This function should be non-blocking.
	//This is NULL at process init, and set when process call the wait func.
	//this is NULL if not waiting for anything
	bool (*check)(void);
  //Pointer to pass to the check function when checking.
  //NB! This can't be stack allocated, as it then won't exist when checking
  //Allocate statically or with malloc!
	INT16U timer; //Software timer for the process
	task_status status;
	struct process *next; //Pointer to next process with same status.
	struct process *prev; //Pointer to previous process with same status.
} process;


//Add the task to the pool. Returns process id. returns 0xff on fail.
void init_scheduler(void);
void start_scheduler(void);
INT8U add_task(void (*process_func)(void));
void remove_task(INT8U id); //Remove the task from the pool
bool check_release(bool (*check)(void));
void wait(INT16U time);
