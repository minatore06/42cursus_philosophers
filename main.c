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
	t_phil	*el;
	t_locks	*locks;
	t_info	*common;

	i = 0;
	if (ft_atoi(argv[1]) < 1)
		return (NULL);
	phils = NULL;
	locks = malloc(sizeof(t_locks));
	pthread_mutex_init(&locks->output, NULL);
	pthread_mutex_init(&locks->forks, NULL);
	pthread_mutex_init(&locks->dead, NULL);
	pthread_mutex_init(&locks->eating_info, NULL);
	common = malloc(sizeof(t_info));
	common->locks = locks;
	common->dead = 0;
	common->phil_eating = ft_atoi(argv[1]);
	while (i < ft_atoi(argv[1]))
	{
		el = malloc(sizeof(t_phil));
		el->id =  i + 1;
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

int	is_dead(long int last_meal, int ttd, t_phil *phil)
{
	struct timeval	now;

	if (get_dead(phil->common->dead, &phil->common->locks->dead))
		return (-1);
	pthread_mutex_lock(&phil->last_meal_lock);
	gettimeofday(&now, NULL);
	/* if (((now.tv_sec * 1000 + now.tv_usec / 1000) - (start.tv_sec * 1000 + start.tv_usec / 1000)) >= 740)
		printf("Boia %d %ld\n", phil->id, ((now.tv_sec * 1000 + now.tv_usec / 1000) - (start.tv_sec * 1000 + start.tv_usec / 1000))); */
	if (last_meal && (now.tv_sec * 1000 + now.tv_usec / 1000) - last_meal >= ttd)
	{
		printf("%ld ~ %ld ~ %d\n", (now.tv_sec * 1000 + now.tv_usec / 1000), last_meal, ttd);
		pthread_mutex_unlock(&phil->last_meal_lock);
		//usleep((ttd - ((now.tv_sec * 1000 + now.tv_usec / 1000) - last_meal)) * 1000);
		output(phil->id, 4, phil->common);
		pthread_mutex_lock(&phil->common->locks->dead);
		phil->common->dead = 1;
		pthread_mutex_unlock(&phil->common->locks->dead);
		return (1);
	}
	pthread_mutex_unlock(&phil->last_meal_lock);
	return (0);
}

void	output(int id, int action, t_info *info)
{
	static struct timeval	start;
	struct timeval	now;

	if (action < 0)
	{
		gettimeofday(&start, NULL);
		return ;
	}
	pthread_mutex_lock(&info->locks->output);
	if (!get_dead(info->dead, &info->locks->dead))
	{
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
			printf("died💀💀💀\n");
	}
	pthread_mutex_unlock(&info->locks->output);
}

void	*becchino(void *phils)
{
	t_phil *phil;

	phil = phils;
	while (!get_dead(phil->common->dead, &phil->common->locks->dead))
	{
		while (phil)
		{
			if (is_dead(phil->last_meal, phil->ttd, phil))
				break ;
			pthread_mutex_lock(&phil->common->locks->eating_info);
			if (!phil->common->phil_eating)
			{
				pthread_mutex_lock(&phil->common->locks->dead);
					phil->common->dead = 1;
				pthread_mutex_unlock(&phil->common->locks->dead);
				break ;
			}
			pthread_mutex_unlock(&phil->common->locks->eating_info);
			phil = phil->next;
		}
		phil = phils;
		usleep(1000);
	}
	printf("sus\n");
/* 	phil = phils;
	while (phil)
	{
		phil->tte = 0;
		phil->tts = 0;
		phil->ttd = 0;
		phil = phil->next;
	} */
	
/* 	while (((t_phil *)phils)->common->dead)
	{
		pthread_mutex_unlock(&((t_phil *)phils)->common->locks->forks);
		pthread_mutex_unlock(&((t_phil *)phils)->common->locks->output);
		manage_forks(1, 1, -1, NULL);
	} */
	return NULL;
}

void	*live_phil(void	*args)
{
	t_phil			*info = (t_phil *)args;
	struct timeval	last_meal;
	struct timeval	now;
	long			now_time;

	pthread_mutex_lock(&info->last_meal_lock);
	gettimeofday(&last_meal, NULL);
	info->last_meal = last_meal.tv_sec * 1000 + last_meal.tv_usec / 1000;
	pthread_mutex_unlock(&info->last_meal_lock);
	while (!get_dead(info->common->dead, &info->common->locks->dead))
	{
		/* if (info->common->dead)
			return (info); */
 		while (manage_forks(-1, 1, info->id, &info->common->locks->forks))
		{
			gettimeofday(&now, NULL);
			now_time = now.tv_sec * 1000 + now.tv_usec / 1000;
			if (now_time- info->last_meal > info->tte / 10)
			{
				if (!manage_forks(-1, 0, info->id, &info->common->locks->forks))
					break ;
			}
			if (get_dead(info->common->dead, &info->common->locks->dead))
			{
				return (info);
			}
		}
		output(info->id, 0, info->common);
		while (manage_forks(-1, -1, info->id, &info->common->locks->forks))
		{
 			if (get_dead(info->common->dead, &info->common->locks->dead))
			{
				manage_forks(1, 0, info->id, &info->common->locks->forks);
				return (info);
			}
		}
		/* if (is_dead(info->last_meal, info->ttd, info->tte, info))
		{
			manage_forks(1, 0, info->id, &info->common->locks->forks);
			return (info);
		} */
		output(info->id, 0, info->common);
		if (info->n_eat)
		{
			info->n_eat++;
			pthread_mutex_lock(&info->common->locks->eating_info);
			if (!info->n_eat)
				info->common->phil_eating--;
			pthread_mutex_unlock(&info->common->locks->eating_info);
		}
		pthread_mutex_lock(&info->last_meal_lock);
		gettimeofday(&last_meal, NULL);
		info->last_meal = last_meal.tv_sec * 1000 + last_meal.tv_usec / 1000;
		pthread_mutex_unlock(&info->last_meal_lock);
		output(info->id, 1, info->common);
		usleep(info->tte * 1000);
		manage_forks(1, 0, info->id, &info->common->locks->forks);
		/* if (info->common->dead)
			return (info); */
		output(info->id, 2, info->common);
		usleep(info->tts * 1000);
		/* if (info->common->dead)
			return (info); */
		output(info->id, 3, info->common);
	}
	//manage_forks(1, 0, info->id, &info->common->locks->forks);
	return (info);
}

void	free_phils(t_phil *phils)
{
	t_phil	*phil;

	pthread_mutex_destroy(&phils->common->locks->dead);
	pthread_mutex_destroy(&phils->common->locks->output);
	pthread_mutex_destroy(&phils->common->locks->forks);
	free(phils->common->locks);
	free(phils->common);
	while (phils)
	{
		phil = phils;
		phils = phils->next;
		free(phil);
	}
}

int main(int argc, char *argv[])
{
	t_phil	*phils;
	t_phil	*tmp;
	pthread_t	bcn;

	if (argc < 5 || argc > 6)
		return (0);
	phils = philosophers_born(argv);
	if (!phils)
		return (0);
	output(0, -1, NULL);
	manage_forks(0, 0, ft_atoi(argv[1]), NULL);
	tmp = phils;
	//phils = last_member(phils);
	while (phils)
	{
		pthread_create(&phils->thread, NULL, &live_phil, phils);
		phils = phils->next;//bfr_this(tmp, phils);
	}
	//phils = tmp;
	phils = last_phil(tmp);
	pthread_create(&bcn, NULL, &becchino, tmp);
	while (phils)
	{
		pthread_join(phils->thread, NULL);
		// manage_forks(1, 1, -1, NULL);
		printf("Phil %d is dead!!!!!!!!!!!!!!!!!!!!!!!\n", phils->id);
		phils = bfr_this(tmp, phils);
	}
	//tmp->common->dead = 0;
	pthread_join(bcn, NULL);
	printf("I'm dead!!!!!!!!!!!!!!!!!!!!!!!\n");
	free_phils(tmp);
/* 	pthread_mutex_destroy(&phils->common->locks->forks);
	pthread_mutex_destroy(&phils->common->locks->output); */
	return (0);
}
