/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scaiazzo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 14:46:36 by scaiazzo          #+#    #+#             */
/*   Updated: 2023/03/15 14:47:02 by scaiazzo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_fork
{
    int     id;
    int     free;
    void    *next;
}t_fork;


typedef struct  s_philosophers
{
    int		id;
    int		ttd;
    int		tte;
    int		tts;
    int		n_eat;
	void	*next;
}t_phil;

int	ft_atoi(char *str);

#endif
