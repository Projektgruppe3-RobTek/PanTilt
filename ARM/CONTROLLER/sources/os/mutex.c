#include "mutex.h"
bool is_mutex_unlocked(mutex_t *mutex)
{
	return (*mutex == UNLOCKED);
}
