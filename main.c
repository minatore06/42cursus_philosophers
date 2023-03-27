/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scaiazzo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 14:47:11 by scaiazzo          #+#    #+#             */
/*   Updated: 2023/03/15 14:47:14 by scaiazzo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philosophers.h"

t_phil  *philosophers_born(char *argv[])
{
	int	i;
	t_phil	*phils;
	t_phil	*el;
	t_locks	*locks;
	t_info	*common;

	i = 0;
	if (ft_atoi(argv[1]) < 1)
		return (NULL);
	phils = NULL;
	locks = malloc(sizeof(t_locks));
	pthread_mutex_init(&locks->output, NULL);
	common = malloc(sizeof(t_info));
	common->locks = locks;
	common->dead = 0;
	while (i < ft_atoi(argv[1]))
	{
		el = malloc(sizeof(t_phil));
		el->id =  i + 1;
		el->ttd = ft_atoi(argv[2]);
		el->tte = ft_atoi(argv[3]);
		el->tts = ft_atoi(argv[4]);
		if (argv[5])
			el->n_eat = -ft_atoi(argv[5]);
		else
			el->n_eat = 1;
		el->common = common;
		if (!phils)
			phils = el;
		else
			last_phil(phils)->next = el;
		i++;
	}
	return (phils);
}

int	is_dead(long int last_meal, int ttd, int shift, t_phil *phil)
{
	struct timeval	now;

	if (phil->common->dead)
		return (-1);
	gettimeofday(&now, NULL);
	if ((now.tv_sec * 1000 + now.tv_usec / 1000) - last_meal + shift >= ttd)
	{
		usleep((ttd - ((now.tv_sec * 1000 + now.tv_usec / 1000) - last_meal)) * 1000);
		output(phil->id, 4, phil->common);
		phil->common->dead = 1;
		return (1);
	}
	return (0);
}

void	output(int id, int action, t_info *info)
{
	static struct timeval	start;
	struct timeval	now;

	if (action < 0)
	{
		gettimeofday(&start, NULL);
		return ;
	}
	pthread_mutex_lock(&info->locks->output);
	if (!info->dead)
	{
		gettimeofday(&now, NULL);
		printf("%ld %d ", ((now.tv_sec * 1000 + now.tv_usec / 1000) - (start.tv_sec * 1000 + start.tv_usec / 1000)), id);
		if (action == 0)
			printf("has taken a fork\n");
		else if (action == 1)
			printf("is eating\n");
		else if (action == 2)
			printf("is sleeping\n");
		else if (action == 3)
			printf("is thinking\n");
		else if (action == 4)
			printf("died💀💀💀\n");
	}
	pthread_mutex_unlock(&info->locks->output);
}

void	*live_phil(void	*args)
{
	t_phil			*info = (t_phil *)args;
	struct timeval	last_meal;

	gettimeofday(&last_meal, NULL);
	info->last_meal = last_meal.tv_sec * 1000 + last_meal.tv_usec / 1000;
	while (!info->common->dead)
	{
		if (is_dead(info->last_meal, info->ttd, 0, info))
			return (info);
/* 		while ()
		{ */
		manage_forks(-1, 1, info->id);
		if (is_dead(info->last_meal, info->ttd, 0, info))
			return (info);
/* 		} */
		output(info->id, 0, info->common);
		info->n_eat++;
/* 		while ()
		{ */
		manage_forks(-1, -1, info->id);
		if (is_dead(info->last_meal, info->ttd, info->tte, info))
			return (info);
/* 		} */
		output(info->id, 1, info->common);
		usleep(info->tte * 1000);
		gettimeofday(&last_meal, NULL);
		info->last_meal = last_meal.tv_sec * 1000 + last_meal.tv_usec / 1000;
		manage_forks(1, 0, info->id);
		if (is_dead(info->last_meal, info->ttd, info->tts, info))
			return (info);
		output(info->id, 2, info->common);
		usleep(info->tts * 1000);
		if (is_dead(info->last_meal, info->ttd, info->tts, info))
			return (info);
		output(info->id, 3, info->common);
	}
	//manage_forks(1, 0, info->id, &info->common->locks->forks);
	return (info);
}

int main(int argc, char *argv[])
{
	t_phil	*phils;
	t_phil	*tmp;

	if (argc < 5 || argc > 6)
		return (0);
	phils = philosophers_born(argv);
	output(0, -1, NULL);
	manage_forks(0, 0, ft_atoi(argv[1]));
	tmp = phils;
	//phils = last_member(phils);
	while (phils)
	{
		pthread_create(&phils->thread, NULL, &live_phil, phils);
		phils = phils->next;//bfr_this(tmp, phils);
	}
	phils = tmp;
	while (phils)
	{
		pthread_join(phils->thread, NULL);
		printf("Phil %d is dead!!!!!!!!!!!!!!!!!!!!!!!\n", phils->id);
		phils = phils->next;
	}
	return (0);
}
