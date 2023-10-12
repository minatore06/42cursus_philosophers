/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scaiazzo <scaiazzo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 14:47:11 by scaiazzo          #+#    #+#             */
/*   Updated: 2023/09/27 11:51:17 by scaiazzo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_phil	*make_phils(t_phil *phils, t_info *common, char *argv[], int i)
{
	t_phil	*el;

	while (i < ft_atoi(argv[1]))
	{
		el = malloc(sizeof(t_phil));
		el->id = i + 1;
		el->ttd = ft_atoi(argv[2]);
		el->tte = ft_atoi(argv[3]);
		el->tts = ft_atoi(argv[4]);
		el->last_meal = 0;
		pthread_mutex_init(&el->last_meal_lock, NULL);
		el->next = NULL;
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

t_phil	*philosophers_born(char *argv[])
{
	t_locks	*locks;
	t_info	*common;

	if (ft_atoi(argv[1]) < 1)
		return (NULL);
	locks = malloc(sizeof(t_locks));
	pthread_mutex_init(&locks->output, NULL);
	pthread_mutex_init(&locks->forks, NULL);
	pthread_mutex_init(&locks->dead, NULL);
	pthread_mutex_init(&locks->eating_info, NULL);
	common = malloc(sizeof(t_info));
	common->locks = locks;
	common->dead = 0;
	common->phil_eating = ft_atoi(argv[1]);
	return (make_phils(NULL, common, argv, 0));
}

void	free_phils(t_phil *phils)
{
	t_phil	*phil;

	pthread_mutex_destroy(&phils->common->locks->dead);
	pthread_mutex_destroy(&phils->common->locks->output);
	pthread_mutex_destroy(&phils->common->locks->forks);
	pthread_mutex_destroy(&phils->common->locks->eating_info);
	free(phils->common->locks);
	free(phils->common);
	while (phils)
	{
		phil = phils;
		phils = phils->next;
		pthread_mutex_destroy(&phil->last_meal_lock);
		free(phil);
	}
}

pthread_t	start_all(char *str, t_phil *phils)
{
	t_phil		*tmp;
	pthread_t	bcn;

	tmp = phils;
	output(0, -1, NULL);
	manage_forks(0, 0, ft_atoi(str), NULL);
	while (phils)
	{
		pthread_create(&phils->thread, NULL, &live_phil, phils);
		phils = phils->next;
	}
	pthread_create(&bcn, NULL, &becchino, tmp);
	return (bcn);
}

int	main(int argc, char *argv[])
{
	t_phil		*phils;
	t_phil		*tmp;
	pthread_t	bcn;

	if (argc < 5 || argc > 6)
		return (0);
	if (ft_atoi(argv[1]) < 1 || ft_atoi(argv[2]) < 0 || ft_atoi(argv[3]) < 0
		|| ft_atoi(argv[4]) < 0 || (argv[5] && ft_atoi(argv[5]) < 0))
		return (0);
	phils = philosophers_born(argv);
	if (!phils)
		return (0);
	bcn = start_all(argv[1], phils);
	tmp = last_phil(phils);
	while (tmp)
	{
		pthread_join(tmp->thread, NULL);
		tmp = bfr_this(phils, tmp);
	}
	pthread_join(bcn, NULL);
	free_phils(phils);
	manage_forks(0, 0, ft_atoi(argv[1]), NULL);
	return (0);
}
