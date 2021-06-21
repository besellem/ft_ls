/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/21 13:07:57 by besellem          #+#    #+#             */
/*   Updated: 2021/06/21 18:58:50 by besellem         ###   ########.fr       */
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
}

int	cmp_node_by_asc(t_node *n1, t_node *n2)
{
	return (ft_strcmp(n2->_dir_.d_name, n1->_dir_.d_name));
}

int	cmp_node_by_desc(t_node *n1, t_node *n2)
{
	return (ft_strcmp(n1->_dir_.d_name, n2->_dir_.d_name));
}

int	cmp_content_asc(void *content_1, void *content_2)
{
	return (ft_strcmp(content_2, content_1));
}

int	cmp_content_desc(void *content_1, void *content_2)
{
	return (ft_strcmp(content_1, content_2));
}


/*
** Sort a list
*/

/*
** OPTI -> THIS SORTING ALGORITHM IS TOO SLOW FOR LARGE LISTS
*/
void	ft_lst_sort(t_list **lst, int (*cmp)())
{
	t_list	*tmp;
	void	*ptr;

	if (!lst || !*lst || !(*cmp))
		return ;
	tmp = *lst;
	while (tmp && tmp->next)
	{
		if (cmp(tmp->content, tmp->next->content) > 0)
		{
			ptr = tmp->content;
			tmp->content = tmp->next->content;
			tmp->next->content = ptr;
			tmp = *lst;
		}
		else
			tmp = tmp->next;
	}
}

void	ft_sort_lst_nodes(t_list **head)
{
	if (is_flag(OPT_R_MIN))
	{
		if (is_flag(OPT_T_MIN))
			ft_lst_sort(head, &cmp_node_by_asc_time);
		else
			ft_lst_sort(head, &cmp_node_by_asc);
	}
	else
	{
		if (is_flag(OPT_T_MIN))
			ft_lst_sort(head, &cmp_node_by_desc_time);
		else
			ft_lst_sort(head, &cmp_node_by_desc);
	}
}
