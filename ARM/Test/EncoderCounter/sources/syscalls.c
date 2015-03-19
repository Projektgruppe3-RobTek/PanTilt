/*Implements the following syscalls:
**sbrk(intptr_t increment)
*/
#include "../headers/syscalls.h"

//defined by the linker. This is the start of the heap.
extern uint32_t end;
//defined by the linker. The heap can't grow beyond this adress.
extern uint32_t HeapLimit;

void *sbrk(intptr_t increment)
{	//Increments the heap according to the argument and returns a pointer to the top of the heap.
	//The heap grows from up, from low adress to high
	static char *heap_end = NULL;
	
	if(heap_end == NULL) heap_end = (char *)&end;
	if(heap_end + increment <= (char *)&HeapLimit)
	{ //If request beyond limit, we don't expand heap. We just return current adress.
		heap_end += increment;
	}
	return (void *)heap_end;
}

void *_sbrk_r(intptr_t increment)
{
	return sbrk(increment);
}
