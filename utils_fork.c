/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_fork.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scaiazzo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 17:08:08 by scaiazzo          #+#    #+#             */
/*   Updated: 2023/09/21 17:08:13 by scaiazzo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philosophers.h"

t_fork	*get_next(t_fork *lst, t_fork *el)
{
	if (el->next)
		return (el->next);
	return (lst);
}

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

t_fork	*last_fork(t_fork *lst)
{
	while (lst)
	{
		if (!lst->next)
			return (lst);
		lst = lst->next;
	}
	return (NULL);
}

t_fork	*bfr_fork(t_fork *lst, t_fork *this)
{
	while (lst->next)
	{
		if (lst->next == this)
			return (lst);
		lst = lst->next;
	}
	return (lst);
}

int	is_one_fork(t_fork *lst)
{
	if (!lst->next)
		return (1);
	return (0);
}
