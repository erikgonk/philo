/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erigonza <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 12:38:54 by erigonza          #+#    #+#             */
/*   Updated: 2024/09/13 12:37:55 by erigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"

unsigned int	ft_get_current_time(void)
{
	struct timeval		tmp;
	unsigned int		current_time;

	gettimeofday(&tmp, NULL);
	current_time = (unsigned int)tmp.tv_sec * 1000 + tmp.tv_usec * 0.001;
	return (current_time);
}

unsigned int	ft_get_moment_time(t_philo *p)
{
	return (ft_get_current_time() - p->t_start);
}

int	ft_dead_check(t_philo p)
{
	int		dead;

	dead = 1;
	pthread_mutex_lock(&p.check_dead);
	if (p.d_flag == 1)
		dead = 0;
	pthread_mutex_unlock(&p.check_dead);
	return (dead);
}

int	ft_printing(t_data *data, int i, int flag, char *action)
{
// flag => is dead?
// action => what action am I printing
	pthread_mutex_lock(&data->print);
	if (ft_dead_check(data->p[i]))
	{
		if (flag == DIE)
		{
			pthread_mutex_lock(&data->p->check_dead);
			data->p->d_flag = 1;
			pthread_mutex_unlock(&data->p->check_dead);
		}
		printf(action, ft_get_moment_time(data->p), data->p[i].id, RESET);
	}
	pthread_mutex_unlock(&data->print);
	return (0);
}

int	ft_save_normi(t_data *data, char  *argv[], int i)
{
	if (data->num < 1)
		return (ft_exit(E_PHILOS));
	data->p[i].num = data->num;
	data->p[i].id = i + 1;
	data->p[i].time = ft_atoll(argv[2]);
	data->p[i].eat = ft_atoll(argv[3]);
	data->p[i].sleep = ft_atoll(argv[4]);
	data->p[i].data = data;
	if (argv[5])
		data->p[i].times_eat = ft_atoll(argv[5]);
	else
		data->p[i].times_eat = -1;
	if (pthread_mutex_init(&data->p[i].fork1, NULL))
		return (ft_exit(E_INIT_T));
	if (pthread_mutex_init(&data->p[i].last_meal, NULL))
		return (ft_exit(E_INIT_T));
	if (pthread_mutex_init(&data->p->check_dead, NULL))
		return (ft_exit(E_INIT_T));
	if (pthread_mutex_init(&data->p->print, NULL))
		return (ft_exit(E_INIT_T));
	if (i > 0)
		data->p[i].fork2 = &data->p[i - 1].fork1;
	return (0);
}
