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
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_fork
{
    int     id;
    int     free;
    pthread_mutex_t lock;
    void    *next;
}t_fork;

typedef struct s_locks
{
    pthread_mutex_t output;
    pthread_mutex_t forks;
    pthread_mutex_t dead;
}t_locks;

typedef struct  s_info
{
    t_locks     *locks;
    int         dead;
}t_info;

typedef struct  s_philosophers
{
    int				id;
    int				ttd;
    int				tte;
    int				tts;
    int				n_eat;
    pthread_mutex_t	n_eat_lock;
    long int		last_meal;
    pthread_mutex_t	last_meal_lock;
    pthread_t		thread;
    t_info			*common;
	void			*next;
}t_phil;


int		get_dead(int dead, pthread_mutex_t *lock);
int		ft_atoi(char *str);
int		last_id(t_fork *lst);
t_phil	*last_phil(t_phil *lst);
t_fork	*last_fork(t_fork *lst);
t_phil	*bfr_this(t_phil *lst, t_phil *this);
t_fork	*get_next(t_fork *lst, t_fork *el);
void	*live_phil(void	*args);
void	output(int id, int action, t_info *info);
int		get_fork(t_fork *forks, int id, int has_fork, pthread_mutex_t *lock);
void	leave_forks(t_fork *forks, int id);
t_fork	*make_forks(int	n);
int 	manage_forks(int action, int hand, int id, pthread_mutex_t *lock);
t_phil	*philosophers_born(char *argv[]);

#endif
