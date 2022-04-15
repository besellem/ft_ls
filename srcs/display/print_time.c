/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_time.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/27 15:22:15 by besellem          #+#    #+#             */
/*   Updated: 2022/04/15 17:13:35 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

#define SIX_MONTHS  15768017 // in seconds

void	print_time(const t_node *node)
{
	const struct stat	_stat = is_flag(OPT_L) ? node->_stats_ : node->_lstats_;
	char				*tmp = NULL;
	char				*file_time = NULL;
	bool				print_year = false;

	if (is_flag(OPT_U_MIN))
	{
		print_year = (time(NULL) - _atime_spec(_stat).tv_sec) >= SIX_MONTHS;
		file_time = ctime(&_atime_spec(_stat).tv_sec);
	}
	else
	{
		print_year = (time(NULL) - _mtime_spec(_stat).tv_sec) >= SIX_MONTHS;
		file_time = ctime(&_mtime_spec(_stat).tv_sec);
	}

	if (!file_time)
		die();
	
	file_time[24] = 0; // removes '\n'

	if (is_flag(OPT_T))
	{
		ft_buffadd(file_time + 4);
		ft_buffaddc(' ');
	}
	else
	{
		if (print_year)
			ft_asprintf(&tmp, "%.*s %s ", 6, file_time + 4, file_time + 19);
		else
			ft_asprintf(&tmp, "%.*s ", 12, file_time + 4);
		if (!tmp)
			die();
		ft_buffadd(tmp);
		ft_memdel((void **)&tmp);
	}
}
