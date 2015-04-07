#include "scheduler.h"
#include "systick.h"
#include "system_tasks.h"
#include "../../headers/assert.h"
#include <stddef.h>
static void init_task(INT8U id, void (*process_func)(void));
static void insert_running(INT8U id);
static void insert_waiting(INT8U id);
static void insert_blocked(INT8U id);
static void remove_from_list(INT8U id, process **list);
static INT8U select_task(void);
static void count_process_timers(void);
static void scheduler_tasks(void);
static bool in_list(INT8U id); //checks if task is in a list, for debugging
static bool in_this_list(INT8U id, process **list); //checks if task is in a specific list, for debugging


static process tasks[MAX_TASKS];

static INT8U current_task = 0;
static process *running[2] = {NULL, NULL}; //pointers to first and last of this process type
static process *waiting[2] = {NULL, NULL}; //pointers to first and last of this process type
static process *blocked[2] = {NULL, NULL}; //pointers to first and last of this process type


static bool in_this_list(INT8U id, process **list)
{
	process *cur = list[0];
	if(list[0] != NULL && list[1] != NULL)
	{
		do
		{
			if(cur->id == id) return true;
			cur = cur-> next;
		}while(cur != list[0]);
	}
	return false;
}


static __attribute__((unused)) bool in_list(INT8U id)
{
	return (in_this_list(id, running) || in_this_list(id, waiting) || in_this_list(id, blocked));
}

static void count_process_timers()
{	//Run throug all process' in waiting and count their timer one down
	if(waiting[0] ==NULL )
		return;

	process *cur =waiting[0];
	do
	{
		if(cur->timer) cur->timer--;
		cur = cur->next;
	}while(cur != waiting[0]);
}

static void scheduler_tasks()
{
	count_process_timers();
}

static INT8U select_task()
{
	INT8U returnid = 0xFF;
	//We start by trying to find a blocked process, they are priotized highest.
	//Find the first one with a function returning true, and start that.
	process *cur = blocked[0];
	while(cur != blocked[1] && !cur->check())
	{
		cur = cur->next;
	}
	if(cur != blocked[1]) //We have found our function, set returnid
	{
		returnid = cur->id;
	}
	else if(blocked[1] != NULL && cur->check())
	{ //We have found our function, set returnid
		returnid = cur->id;
	}
	if(returnid == 0xFF)
	{	//If we didn't find a blocking one, try to find a waiting which is ready.
		//They should be in order, so we actually only need to try the first one
		if(waiting[0] != NULL && waiting[0]->timer == 0)
		{
			returnid = waiting[0]->id;
		}
	}
	if(returnid == 0xFF && running[0] != NULL)
	{//If we still didn't find one, find from running tasks.
			returnid = running[0]->id;
	}
	//remove from current queue and set status to running
	if(returnid != 0xFF)
	{
		switch(tasks[returnid].status)
		{
			case RUNNING:
				remove_from_list(returnid, running);
			break;
			case WAITING:
				remove_from_list(returnid, waiting);
			break;
			case BLOCKED:
				remove_from_list(returnid, blocked);
			break;
			default:
			break;
		}
		tasks[returnid].status = RUNNING;
	}
	return returnid;
}




void start_scheduler()
{
	while(1)
	{
		while(ticks)
		{
			ticks--;
			scheduler_tasks();
			system_tasks();
		}
		current_task = select_task();
		if(current_task != 0xFF)
		{
			tasks[current_task].process_func();
			if(tasks[current_task].status == RUNNING)
				insert_running(current_task);
		}
	}
}

bool check_release(bool (*check)(void))
{

	if(check())
	{
		return true;
	}
	else
	{
		tasks[current_task].check = check;
		tasks[current_task].status = BLOCKED;
		insert_blocked(tasks[current_task].id);
		return false;
	}
}

void wait(INT16U time)
{ //This process wants to wait some time.
	//Put the time in its timer, inset it in the waiting list and return
	//to the process.
	tasks[current_task].timer = time;
	tasks[current_task].status = WAITING;
	insert_waiting(current_task);
}

void init_scheduler(void)
{
	for (INT8U i = 0; i < MAX_TASKS; i += 1)
	{
		tasks[i].id = i;
		tasks[i].process_func = NULL;
		tasks[i].check = NULL;
		tasks[i].timer = 0;
		tasks[i].status = DEAD;
		running[0] = NULL;
		running[1] = NULL;
		waiting[0] = NULL;
		waiting[1] = NULL;
		blocked[0] = NULL;
		blocked[1] = NULL;

	}
}

static void init_task(INT8U id, void (*process_func)(void))
{
	tasks[id].id = id;
	tasks[id].process_func = process_func;
	tasks[id].check = NULL;
	tasks[id].timer = 0;
	tasks[id].status = RUNNING;
	insert_running(id);
}

INT8U add_task(void (*process_func)(void))
{
	INT8U id = 0xFF;
	for(INT8U i = 0; i < MAX_TASKS && id == 0xFF; i++)
	{
		if(tasks[i].status == DEAD)
		{
			id = i;
			init_task(id, process_func);
		}
	}
	return id;
}

void remove_task(INT8U id)
{
	switch(tasks[id].status)
	{
		case RUNNING:
			remove_from_list(id, running);
		break;
		case WAITING:
			remove_from_list(id, waiting);
		break;
		case BLOCKED:
			remove_from_list(id, blocked);
		break;
		default:
		break;
	}
	tasks[id].status = DEAD;
}

static void insert_running(INT8U id)
{
	assert(!in_this_list(id, running));
	if(running[0] == NULL) //this should only happen if no jobs are running
	{
		//Set prev and next to point to itself, and set running[0:1] to point to it also.
		tasks[id].next = &tasks[id];
		tasks[id].prev = &tasks[id];
		running[0] = &tasks[id];
		running[1] = &tasks[id];
	}
	else
	{
		//Else, we insert at end of queue
		running[1]->next = &tasks[id];
		tasks[id].prev = running[1];
		tasks[id].next = running[0];
		running[0]->prev = &tasks[id];
		running[1] = &tasks[id];
	}
	assert(in_this_list(id, running));
}

static void insert_blocked(INT8U id)
{	//this code should be excatly the same as insert running
	//But we don't combine it, as it would make it overly complicated.
	//And it's important that there is not bugs in this code.
	assert(!in_this_list(id, blocked));

	if(blocked[0] == NULL) //this should only happen if no jobs are blocked
	{
		//Set prev and next to point to itself, and set blocked[0:1] to point to it also.
		tasks[id].next = &tasks[id];
		tasks[id].prev = &tasks[id];
		blocked[0] = &tasks[id];
		blocked[1] = &tasks[id];
	}
	else
	{ //Else, we insert at end of queue
		blocked[1]->next = &tasks[id];
		tasks[id].prev = blocked[1];
		tasks[id].next = blocked[0];
		blocked[0]->prev = &tasks[id];
		blocked[1] = &tasks[id];
	}
	assert(in_this_list(id, blocked));
}
static void insert_waiting(INT8U id)
{ /* THERE MAY BE BUGS IN THIS CODE!!! */
	assert(!in_this_list(id, waiting));
	if(waiting[0] == NULL) //this should only happen if no jobs are waiting
	{
		//Set prev and next to point to itself, and set waiting[0:1] to point to it also.
		tasks[id].next = &tasks[id];
		tasks[id].prev = &tasks[id];
		waiting[0] = &tasks[id];
		waiting[1] = &tasks[id];
	}
	else
	{ //Here we travel the queue and find where to put the process.
		//The queue should be in a sequence ordered after the timer value.
		//If there are values equal to the process we insert, the inserted process
		//Is placed last.
		process *cur = waiting[0];
		while(cur != waiting[1] && cur->timer <= tasks[id].timer)
		{
			cur = cur->next;
		}
		//eighter, cur is now pointing on the end of the list,
		//the first value with a timer larger than the inserted,
		//or both. Find out which one
		//We also need to check if the timer should be inserted at the start of the queue
		if(cur == waiting[1] && cur->timer <= tasks[id].timer)
		{ //place the task at the end of the list
			waiting[1]->next = &tasks[id];
			tasks[id].prev = waiting[1];
			tasks[id].next = waiting[0];
			waiting[0]->prev = &tasks[id];
			waiting[1] = &tasks[id];
			assert(in_this_list(id, waiting));
			return;
		}
		else if(cur->timer > tasks[id].timer)
		{ //travel one step back
			cur = cur-> prev;
		}
		//check if the task should be inserted at the start of the queue
		if(cur-> next == waiting[0] )
		{	//Insert at queue start
			///BEWARE OF BUGS HERE///
			tasks[id].next = cur->next;
			tasks[id].prev = cur;
			waiting[1]->next = &tasks[id];
			cur->prev = &tasks[id];
			waiting[0] = &tasks[id];
		}
		else
		{ //We just insert after cur
			tasks[id].next = cur->next;
			tasks[id].prev = cur;
			cur -> next = &tasks[id];
		}
		assert(in_this_list(id, waiting));
	}
}
static void remove_from_list(INT8U id, process **list) //united function as they are exctely the same
{
	assert(in_this_list(id, list));
	//Travel the list until we reach the id. This may take long time,
	//But most of the time the task will be at the begining.
	process *cur = list[0];
	while( cur->id != id && cur != list[1] )
	{//We eighter reach the last or the correct one
		cur = cur -> next;
	}
	assert(cur -> id == id); //we got the correct one!
	if(cur == list[0] && cur == list[1])
	{ //this means we are the only process
		list[0] = NULL;
		list[1] = NULL;
	}
	else if(cur == list[0])
	{ //We are the first process
		list[0] = cur->next;
		list[0]->prev = cur->prev;
		cur->prev->next = list[0];
	}
	else if(cur == list[1])
	{//We are the last process
		list[1] = cur->prev;
		cur->next->prev = cur->prev;
		cur->prev->next = cur->next;
	}
	else
	{//We just remove this process
		cur->next->prev = cur->prev;
		cur->prev->next = cur->next;
	}
	assert(!in_this_list(id, list));
}
