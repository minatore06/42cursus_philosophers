#include "philosophers.h"

int get_dead(int dead, pthread_mutex_t *lock)
{
    int result;

	pthread_mutex_lock(lock);
    result = dead;
	pthread_mutex_unlock(lock);
    return (result);
}