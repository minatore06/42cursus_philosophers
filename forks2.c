/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scaiazzo <scaiazzo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 11:14:51 by scaiazzo          #+#    #+#             */
/*   Updated: 2023/09/27 11:52:29 by scaiazzo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_fork	*make_forks(int n)
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
		el->next = NULL;
		if (!forks)
			forks = el;
		else
			last_fork(forks)->next = el;
		i++;
	}
	return (forks);
}

void	destroy_forks(t_fork *forks)
{
	t_fork	*fork;

	while (forks)
	{
		fork = forks;
		forks = forks->next;
		pthread_mutex_destroy(&fork->lock);
		free(fork);
	}
}

t_fork	*oppenheimer(int id, t_fork *forks)
{
	if (!forks)
		return (make_forks(id));
	destroy_forks(forks);
	return (NULL);
}
