/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_phil.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scaiazzo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 17:07:58 by scaiazzo          #+#    #+#             */
/*   Updated: 2023/09/21 17:08:02 by scaiazzo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philosophers.h"

t_phil	*last_phil(t_phil *lst)
{
	while (lst)
	{
		if (!lst->next)
			return (lst);
		lst = lst->next;
	}
	return (NULL);
}

t_phil	*bfr_this(t_phil *lst, t_phil *this)
{
	while (lst->next)
	{
		if (lst->next == this)
			return (lst);
		lst = lst->next;
	}
	return (NULL);
}
