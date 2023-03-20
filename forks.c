#include "philosophers.h"

t_fork	*make_forks(int	n)
{
	t_fork	*forks;
	t_fork	*current;
	t_fork	*previous;
	int		i;

	forks = malloc(sizeof(t_fork));
	current = forks;
	previous = forks;
	i = 0;
	while (i < n)
	{
		current->id = i + 1;
		current->free = 1;
		current->next = 0;
		if (i != 0)
			previous->next = current;
		if (i != 0)
			previous = previous->next;
		current = malloc(sizeof(t_fork));
		i++;
	}
	free(current);
	return (forks);
}

int	get_fork(t_fork *forks, int id)
{
	while (forks)
	{
		if (forks->id == id)
		{
			if (forks->free)
			{
				forks->free = 0;
				return (0);
			}
			else
				return (1);
		}
		forks = forks->next;
	}
	return (1);
}

void	leave_forks(t_fork *forks, int id)
{
	while (forks)
	{
		if (forks->id == id)
			forks->free = 1;
		else if (forks->id == id + 1)
			forks->free = 1;
		forks = forks->next;
	}
}

void	manage_forks(int action, int hand, int id, pthread_mutex_t *lock)
{
	static t_fork	*forks;
    int             free;

	if (!hand && !action)
		forks = make_forks(id);
	if (action < 0)
	{
		if (hand < 0)
		{
			while (free)
            {
                pthread_mutex_lock(lock);
                free = get_fork(forks, id + 1);
                pthread_mutex_unlock(lock);
            }
		}
		else if (hand > 0)
		{
			while (free)
            {
                pthread_mutex_lock(lock);
                free = get_fork(forks, id);
                pthread_mutex_unlock(lock);
            }
		}
	}
	else if (action > 0)
	{
        pthread_mutex_lock(lock);
		leave_forks(forks, id);
        pthread_mutex_unlock(lock);
	}
}
