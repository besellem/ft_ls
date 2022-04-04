/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_time.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/27 15:22:15 by besellem          #+#    #+#             */
/*   Updated: 2022/04/04 17:35:59 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	print_time(const t_node *node)
{
	char	*file_time = ctime(&node->_lstats_.st_mtimespec.tv_sec);

	if (!file_time)
		return ;
	file_time[16] = '\0';		/* don't print after 16th char (man ctime) */
	ft_buffadd(file_time + 4);	/* skip the day (first 4 chars) */
	ft_buffaddc(' ');
}
