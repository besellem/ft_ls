/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/21 13:07:57 by besellem          #+#    #+#             */
/*   Updated: 2022/04/04 15:07:24 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/*
** Comparision functions - to use with lists
*/

int	cmp_node_by_asc_time(t_node *n1, t_node *n2)
{
	const time_t	n1_sec = n1->_stats_.st_mtimespec.tv_sec;
	const time_t	n2_sec = n2->_stats_.st_mtimespec.tv_sec;
	
	if (n1_sec == n2_sec)
		return (n1->_stats_.st_mtimespec.tv_nsec > n2->_stats_.st_mtimespec.tv_nsec);
	return (n1_sec > n2_sec);
}

int	cmp_node_by_desc_time(t_node *n1, t_node *n2)
{
	const time_t	n1_sec = n1->_stats_.st_mtimespec.tv_sec;
	const time_t	n2_sec = n2->_stats_.st_mtimespec.tv_sec;
	
	if (n1_sec == n2_sec)
		return (n1->_stats_.st_mtimespec.tv_nsec < n2->_stats_.st_mtimespec.tv_nsec);
	return (n1_sec < n2_sec);
	
	// return (cmp_node_by_asc_time(n2, n1)); // may be a valid solution, to test
}

int	cmp_node_by_asc(t_node *n1, t_node *n2)
{
	// printf("n1[%s] n2[%s] (n2 - n1)[%d]\n",
	// 	n1->_dir_.d_name, n2->_dir_.d_name,
	// 	ft_strcmp(n2->_dir_.d_name, n1->_dir_.d_name));
	return (ft_strcmp(n1->_dir_.d_name, n2->_dir_.d_name));
}

int	cmp_node_by_desc(t_node *n1, t_node *n2)
{
	return (ft_strcmp(n2->_dir_.d_name, n1->_dir_.d_name));
}

int	cmp_content_asc(void *content_1, void *content_2)
{
	return (ft_strcmp(content_2, content_1));
}

int	cmp_content_desc(void *content_1, void *content_2)
{
	return (ft_strcmp(content_1, content_2));
}

void	ft_sort_lst_nodes(t_list **head)
{
	if (is_flag(OPT_R_MIN))
	{
		if (is_flag(OPT_T_MIN))
			ft_lst_qsort(head, &cmp_node_by_asc_time);
		else
			ft_lst_qsort(head, &cmp_node_by_desc);
	}
	else
	{
		if (is_flag(OPT_T_MIN))
			ft_lst_qsort(head, &cmp_node_by_desc_time);
		else
			ft_lst_qsort(head, &cmp_node_by_asc);
	}
}
