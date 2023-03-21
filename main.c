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
	t_phil	*current;
	t_phil	*previous;
	t_locks	*locks;
	t_info	*common;

	i = 0;
	phils = malloc(sizeof(t_phil));
	current = phils;
	previous = phils;
	locks = malloc(sizeof(t_locks));
	pthread_mutex_init(&locks->forks, NULL);
	pthread_mutex_init(&locks->output, NULL);
	common = malloc(sizeof(t_info));
	common->locks = locks;
	common->dead = 0;
	while (i < ft_atoi(argv[1]))
	{
		current->id =  i + 1;
		current->ttd = ft_atoi(argv[2]);
		current->tte = ft_atoi(argv[3]);
		current->tts = ft_atoi(argv[4]);
		if (argv[5])
			current->n_eat = ft_atoi(argv[5]);
		else
			current->n_eat = -1;
		current->common = common;
		current->next = 0;
		if (i != 0)
			previous->next = current;
		if (i != 0)
			previous = previous->next;
		current = malloc(sizeof(t_phil));
		i++;
	}
	free(current);
	return (phils);
}

int	is_dead(long int last_meal, int ttd, int shift, int dead)
{
	struct timeval	now;

	if (dead)
		return (-1);
	gettimeofday(&now, NULL);
	if ((now.tv_sec * 1000 + now.tv_usec / 1000) - last_meal + shift >= ttd)
		return (1);
	return (0);
}

void	output(int id, int action, pthread_mutex_t *lock)
{
	static struct timeval	start;
	struct timeval	now;

	if (action < 0)
	{
		gettimeofday(&start, NULL);
		return ;
	}
	pthread_mutex_lock(lock);
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
		printf("died\n");
	pthread_mutex_unlock(lock);
}

void	*live_phil(void	*args)
{
	t_phil	*info = (t_phil *)args;
	struct timeval	last_meal;

	gettimeofday(&last_meal, NULL);
	info->last_meal = last_meal.tv_sec * 1000 + last_meal.tv_usec / 1000;
	while (!info->common->dead)
	{
		if (is_dead(info->last_meal, info->ttd, 0, info->common->dead))
			break ;
		manage_forks(-1, 1, info->id, &info->common->locks->forks);
		if (is_dead(info->last_meal, info->ttd, 0, info->common->dead))
			break ;
		output(info->id, 0, &info->common->locks->output);
		manage_forks(-1, -1, info->id, &info->common->locks->forks);
 		if (is_dead(info->last_meal, info->ttd, info->tte, info->common->dead))
			break ;
		output(info->id, 1, &info->common->locks->output);
		usleep(info->tte * 1000);
		gettimeofday(&last_meal, NULL);
		info->last_meal = last_meal.tv_sec * 1000 + last_meal.tv_usec / 1000;
		manage_forks(1, 0, info->id, &info->common->locks->forks);
		if (is_dead(info->last_meal, info->ttd, info->tts, info->common->dead))
			break ;
		output(info->id, 2, &info->common->locks->output);
		usleep(info->tts * 1000);
		output(info->id, 3, &info->common->locks->output);
		info->n_eat--;
	}
	if (info->n_eat)
	{
		output(info->id, 4, &info->common->locks->output);
		info->common->dead = 1;
	}
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
	manage_forks(0, 0, ft_atoi(argv[1]), NULL);
	tmp = phils;
	while (phils)
	{
		pthread_create(&phils->thread, NULL, &live_phil, phils);
		phils = phils->next;
	}
	phils = tmp;
	while (phils)
	{
		pthread_join(phils->thread, NULL);
		phils = phils->next;
	}
	return (0);
}
