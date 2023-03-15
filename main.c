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

	i = 0;
	phils = malloc(sizeof(t_phil));
	current = phils;
	previous = phils;
	while (i < ft_atoi(argv[1]))
	{
		current->id =  i + 1;
		current->ttd = ft_atoi(argv[2]);
		current->tte = ft_atoi(argv[3]);
		current->tts = ft_atoi(argv[4]);
		if (argv[5])
			current->n_eat = ft_atoi(argv[5]);
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

	manage_forks(-1, 1, info->id);
	output(info->id, 0);
	manage_forks(-1, -1, info->id);
	output(info->id, 1);
	usleep(info->tte);
	manage_forks(1, 0, info->id);
	output(info->id, 2);
	usleep(info->tts);
	output(info->id, 3);
}

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

void	manage_forks(int action, int hand, int id)
{
	static t_fork	*forks;

	if (!hand && !action)
		forks = make_forks(id);
	if (action < 0)
	{
		if (hand < 0)
		{
			get_fork(forks, id + 1);
		}
		else if (hand > 0)
		{
			get_fork(forks, id);
		}
	}
	else if (action > 0)
	{
		leave_forks(forks, id);
	}
}

int main(int argc, char *argv[])
{
	t_phil	*phils;
	t_fork	*forks;

	if (argc < 5 || argc > 6)
		return (0);
	phils = philosophers_born(argv);
	output(0, -1);
	while (phils)
	{
		printf("Philosopher: %d\n", phils->id);
		phils = phils->next;
	}
	return (0);
}
