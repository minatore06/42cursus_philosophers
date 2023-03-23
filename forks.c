#include "philosophers.h"

t_fork	*make_forks(int	n)
{
	t_fork	*forks;
	t_fork	*current;
	int		i;

	forks = malloc(sizeof(t_fork));
	current = forks;
	i = 0;
	while (i < n)
	{

		pthread_mutex_init(&current->free, NULL);
		current->id = i + 1;
		current->next = malloc(sizeof(t_fork));
		current = current->next;
		i++;
	}
	free(current);
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

int	get_fork(t_fork *forks, int id, int has_fork)
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
/* 			if (last_fork(forks) && !has_fork)
				return (1);
			if (!has_fork && !get_next(forks, fork)->free)
				return (1); */
			pthread_mutex_lock(&fork->free);
			return (0);
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
			pthread_mutex_unlock(&fork->free);
		}
		else if (fork->id == id + 1)
		{
			count--;
			pthread_mutex_unlock(&fork->free);
		}
		fork = fork->next;
	}
	if (count)
		pthread_mutex_unlock(&fork->free);
}

int	manage_forks(int action, int hand, int id)
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
			free = get_fork(forks, id + 1, 1);
			return (free);
		}
		else if (hand > 0)
		{
			free = get_fork(forks, id, 0);
			return (free);
		}
	}
	else if (action > 0)
	{
		leave_forks(forks, id);
	}
	return (0);
}
