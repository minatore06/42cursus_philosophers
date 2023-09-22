/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fred.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scaiazzo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 11:51:57 by scaiazzo          #+#    #+#             */
/*   Updated: 2023/09/22 11:52:00 by scaiazzo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philosophers.h"

int	eating_pasta(t_phil *info)
{
	long	now_time;

	while (manage_forks(-1, 1, info->id, &info->common->locks->forks))
	{
		now_time = get_time();
		if (now_time - info->last_meal > info->tte / 10)
		{
			if (!manage_forks(-1, 0, info->id, &info->common->locks->forks))
				break ;
		}
		if (get_dead(info->common->dead, &info->common->locks->dead))
			return (1);
	}
	output(info->id, 0, info->common);
	while (manage_forks(-1, -1, info->id, &info->common->locks->forks))
	{
		if (get_dead(info->common->dead, &info->common->locks->dead))
		{
			manage_forks(1, 0, info->id, &info->common->locks->forks);
			return (1);
		}
	}
	output(info->id, 0, info->common);
	return (0);
}

void	update_info(t_phil *info)
{
	pthread_mutex_lock(&info->last_meal_lock);
	info->last_meal = get_time();
	pthread_mutex_unlock(&info->last_meal_lock);
	output(info->id, 1, info->common);
	usleep(info->tte * 1000);
	if (info->n_eat)
	{
		info->n_eat++;
		pthread_mutex_lock(&info->common->locks->eating_info);
		if (!info->n_eat)
			info->common->phil_eating--;
		pthread_mutex_unlock(&info->common->locks->eating_info);
	}
}

void	*live_phil(void	*args)
{
	t_phil			*info;

	info = (t_phil *)args;
	pthread_mutex_lock(&info->last_meal_lock);
	info->last_meal = get_time();
	pthread_mutex_unlock(&info->last_meal_lock);
	while (!get_dead(info->common->dead, &info->common->locks->dead))
	{
		if (eating_pasta(info))
			return (info);
		update_info(info);
		manage_forks(1, 0, info->id, &info->common->locks->forks);
		output(info->id, 2, info->common);
		usleep(info->tts * 1000);
		output(info->id, 3, info->common);
	}
	return (info);
}
