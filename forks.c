#include "philosophers.h"

t_fork	*make_forks(int	n)
{
	t_fork	*forks;
	t_fork	*el;
	int		i;

	forks = NULL;
	i = 0;
	while (i < n)
	{
		el = malloc(sizeof(t_fork));
		el->free = 1;
		pthread_mutex_init(&el->lock, NULL);
		el->id = i + 1;
		if (!forks)
			forks = el;
		else
			last_fork(forks)->next = el;
		i++;
	}
	return (forks);
}

/* int	last_fork(t_fork *forks)
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
} */

/* void	print_forks(t_fork *forks)
{
	while (forks)
	{
		printf("Fork %d, free %d\n", forks->id, forks->free);
		forks = forks->next;
	}
} */

int	get_fork(t_fork *forks, int id, int has_fork, pthread_mutex_t *lock)
{
	t_fork	*fork;

	(void)has_fork;
	if (id > last_id(forks))
		id = 1;
	fork = forks;
	while (fork)
	{
		if (fork->id == id)
		{
 			if (last_fork(forks) && !has_fork)
				return (1);
/* 			if (!has_fork && !get_next(forks, fork)->free)
				return (1); */
			printf("sos\n");
			pthread_mutex_unlock(lock);
			pthread_mutex_lock(&fork->lock);
			fork->free = 0;
			return (0);
		}
		fork = get_next(forks, fork);
	}
	return (1);
}

void	leave_forks(t_fork *forks, int id, pthread_mutex_t *lock)
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
			pthread_mutex_unlock(&fork->lock);
		}
		else if (fork->id == id + 1)
		{
			count--;
			fork->free = 1;
			pthread_mutex_unlock(&fork->lock);
		}
		fork = fork->next;
	}
	if (count)
	{
		fork->free = 1;
		pthread_mutex_unlock(&fork->lock);
	}
	pthread_mutex_unlock(lock);
}

int	manage_forks(int action, int hand, int id, pthread_mutex_t *lock)
{
	static t_fork	*forks;
    int             free;

	if (!hand && !action)
	{
		forks = make_forks(id);
		return (0);
	}
	free = 1;
	pthread_mutex_lock(lock);
	if (action < 0)
	{
		if (hand < 0)
			free = get_fork(forks, id + 1, 1, lock);
		else if (hand > 0)
			free = get_fork(forks, id, 0, lock);
	}
	else if (action > 0)
		leave_forks(forks, id, lock);
	return (free);
}
