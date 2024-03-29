/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   friends.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scaiazzo <scaiazzo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 15:42:27 by scaiazzo          #+#    #+#             */
/*   Updated: 2023/09/27 11:50:53 by scaiazzo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	is_dead(int ttd, t_phil *phil)
{
	long		now;
	long int	last_meal;

	if (get_dead(phil->common, &phil->common->locks->dead))
		return (-1);
	pthread_mutex_lock(&phil->last_meal_lock);
	last_meal = phil->last_meal;
	now = get_time();
	if (last_meal && now - last_meal >= ttd)
	{
		pthread_mutex_unlock(&phil->last_meal_lock);
		output(phil->id, 4, phil->common);
		pthread_mutex_lock(&phil->common->locks->dead);
		phil->common->dead = 1;
		pthread_mutex_unlock(&phil->common->locks->dead);
		return (1);
	}
	pthread_mutex_unlock(&phil->last_meal_lock);
	return (0);
}

void	output(int id, int action, t_info *info)
{
	static long	start;
	long		now;

	if (action < 0)
	{
		start = get_time();
		return ;
	}
	pthread_mutex_lock(&info->locks->output);
	if (!get_dead(info, &info->locks->dead))
	{
		now = get_time();
		printf("%ld %d ", (now - start), id);
		if (action == 0)
			printf("has taken a fork\n");
		else if (action == 1)
			printf("is eating\n");
		else if (action == 2)
			printf("is sleeping\n");
		else if (action == 3)
			printf("is thinking\n");
		else if (action == 4)
			printf("died\n");
	}
	pthread_mutex_unlock(&info->locks->output);
}

void	*becchino(void *phils)
{
	t_phil	*phil;

	phil = phils;
	while (!get_dead(phil->common, &phil->common->locks->dead))
	{
		while (phil)
		{
			if (is_dead(phil->ttd, phil))
				break ;
			pthread_mutex_lock(&phil->common->locks->eating_info);
			if (!phil->common->phil_eating)
			{
				pthread_mutex_lock(&phil->common->locks->dead);
					phil->common->dead = 1;
				pthread_mutex_unlock(&phil->common->locks->dead);
				break ;
			}
			pthread_mutex_unlock(&phil->common->locks->eating_info);
			phil = phil->next;
		}
		phil = phils;
		usleep(1000);
	}
	return (NULL);
}
