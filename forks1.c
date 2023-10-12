/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scaiazzo <scaiazzo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 17:07:46 by scaiazzo          #+#    #+#             */
/*   Updated: 2023/10/11 17:27:12 by scaiazzo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	mega_check(t_fork *forks, t_fork *fork, int has_fork)
{
	if ((!(last_fork(forks)->id % 2) || last_fork(forks)->id == 3)
		&& islast_fork(forks) && has_fork <= 0)
		return (1);
	if (!islast_fork(forks) && has_fork <= 0
		&& !get_next(forks, fork)->free)
		return (1);
	if (!islast_fork(forks) && !has_fork
		&& !bfr_fork(forks, fork)->free)
		return (1);
	return (0);
}

int	get_fork(t_fork *forks, int id, int has_fork, pthread_mutex_t *lock)
{
	t_fork	*fork;

	if (id > last_id(forks) && !is_one_fork(forks))
		id = 1;
	fork = forks;
	while (fork)
	{
		if (fork->id == id)
		{
			if (mega_check(forks, fork, has_fork))
				return (1);
			fork->free = 0;
			pthread_mutex_unlock(lock);
			pthread_mutex_lock(&fork->lock);
			pthread_mutex_lock(lock);
			fork->free = 0;
			pthread_mutex_unlock(lock);
			return (0);
		}
		fork = fork->next;
	}
	return (1);
}

void	leave_forks(t_fork *forks, int id, int full)
{
	t_fork	*fork;

	fork = forks;
	while (fork)
	{
		if (fork->id == id)
		{
			fork->free = 1;
			pthread_mutex_unlock(&fork->lock);
			if (full)
			{
				get_next(forks, fork)->free = 1;
				pthread_mutex_unlock(&get_next(forks, fork)->lock);
			}
			return ;
		}
		fork = fork->next;
	}
}

int	manage_forks(int action, int hand, int id, pthread_mutex_t *lock)
{
	static t_fork	*forks;
	int				free;

	if (!hand && !action)
		forks = oppenheimer(id, forks);
	if (!hand && !action)
		return (0);
	free = 1;
	pthread_mutex_lock(lock);
	if (action < 0)
	{
		if (id > last_id(forks))
			return (1);
		if (hand < 0)
			free = get_fork(forks, id + 1, 1, lock);
		else if (hand > 0)
			free = get_fork(forks, id, 0, lock);
		else
			free = get_fork(forks, id, -1, lock);
	}
	else if (action > 0)
		leave_forks(forks, id, hand);
	if (free)
		pthread_mutex_unlock(lock);
	return (free);
}
