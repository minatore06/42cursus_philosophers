/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_fork2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scaiazzo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 17:50:40 by scaiazzo          #+#    #+#             */
/*   Updated: 2023/09/22 17:50:42 by scaiazzo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philosophers.h"

int	last_id(t_fork *lst)
{
	int	id;

	while (lst)
	{
		id = lst->id;
		lst = lst->next;
	}
	return (id);
}

int	islast_fork(t_fork *forks)
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
}

int	is_one_fork(t_fork *lst)
{
	if (!lst->next)
		return (1);
	return (0);
}
