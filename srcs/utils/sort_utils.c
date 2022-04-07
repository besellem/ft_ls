/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/21 13:07:57 by besellem          #+#    #+#             */
/*   Updated: 2022/04/06 20:38:49 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/*
** Comparision functions - used on nodes
*/

inline int		cmp_node_by_asc_time(t_node *n1, t_node *n2)
{
	const time_t	n1_sec = n1->_stats_.st_mtimespec.tv_sec;
	const time_t	n2_sec = n2->_stats_.st_mtimespec.tv_sec;
	
	if (n1_sec == n2_sec)
		return (n1->_stats_.st_mtimespec.tv_nsec > n2->_stats_.st_mtimespec.tv_nsec);
	return (n1_sec > n2_sec);
}

inline int		cmp_node_by_desc_time(t_node *n1, t_node *n2)
{
	return (cmp_node_by_asc_time(n2, n1));
}

inline int		cmp_node_by_asc(t_node *n1, t_node *n2)
{
	return (ft_strcmp(n1->_dir_.d_name, n2->_dir_.d_name));
}

inline int		cmp_node_by_desc(t_node *n1, t_node *n2)
{
	return (cmp_node_by_asc(n2, n1));
}
