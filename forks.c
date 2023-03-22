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

int	last_fork(t_fork *forks)
{
	int	count;

	count = 0;
	while (forks)
	{
		if (forks->free)
			count++;
		forks = forks->next;
	}
	if (count == 1)
		return (1);
	return (0);
}

void	print_forks(t_fork *forks)
{
	while (forks)
	{
		printf("Fork %d, free %d\n", forks->id, forks->free);
		forks = forks->next;
	}
}

int	get_fork(t_fork *forks, int id, int has_fork)
{
	t_fork	*fork;

	if (id > last_id(forks))
		id = 1;
	fork = forks;
	while (fork)
	{
		if (fork->id == id)
		{
			//if (!has_fork && !get_next(forks, fork)->free)
			if (last_fork(forks) && !has_fork)
				return (1);
			if (fork->free)
			{
				fork->free = 0;
				return (0);
			}
			else
				return (1);
		}
		fork = get_next(forks, fork);
	}
	return (1);
}

void	leave_forks(t_fork *forks, int id)
{
	t_fork	*fork;
	int		count;

	fork = forks;
	count = 2;
	while (fork)
	{
		if (fork->id == id)
		{
			count--;
			fork->free = 1;
		}
		else if (fork->id == id + 1)
		{
			count--;
			fork->free = 1;
		}
		fork = fork->next;
	}
	if (count)
		forks->free = 1;
}

int	manage_forks(int action, int hand, int id, pthread_mutex_t *lock)
{
	static t_fork	*forks;
    int             free;

	if (!hand && !action)
		forks = make_forks(id);
	free = 1;
	if (action < 0)
	{
		if (hand < 0)
		{
			pthread_mutex_lock(lock);
			free = get_fork(forks, id + 1, 1);
			pthread_mutex_unlock(lock);
			return (free);
		}
		else if (hand > 0)
		{
			pthread_mutex_lock(lock);
			free = get_fork(forks, id, 0);
			pthread_mutex_unlock(lock);
			return (free);
		}
	}
	else if (action > 0)
	{
        pthread_mutex_lock(lock);
		leave_forks(forks, id);
        pthread_mutex_unlock(lock);
	}
	return (0);
}
