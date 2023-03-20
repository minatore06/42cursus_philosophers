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

	i = 0;
	phils = malloc(sizeof(t_phil));
	current = phils;
	previous = phils;
	locks = malloc(sizeof(t_locks));
	pthread_mutex_init(locks->forks, NULL);
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
		current->locks = locks;
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

void	output(int id, int action)
{
	static struct timeval	start;
	struct timeval	now;

	if (action < 0)
	{
		gettimeofday(&start, NULL);
		return ;
	}
	gettimeofday(&now, NULL);
	printf("%ld %d ", (now.tv_usec - start.tv_usec) / 1000, id);
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

void	*live_phil(void	*args)
{
	t_phil	*info = (t_phil *)args;

	while (info->n_eat)
	{
		manage_forks(-1, 1, info->id, info->locks->forks);
		output(info->id, 0);
		manage_forks(-1, -1, info->id, info->locks->forks);
		output(info->id, 1);
		usleep(info->tte);
		manage_forks(1, 0, info->id, info->locks->forks);
		output(info->id, 2);
		usleep(info->tts);
		output(info->id, 3);
		info->n_eat--;
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
	output(0, -1);
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
