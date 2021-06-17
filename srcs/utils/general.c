/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 22:22:52 by besellem          #+#    #+#             */
/*   Updated: 2021/06/17 18:52:19 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int		is_flag(uint64_t flag)
{
	return (singleton()->opts & flag);
}

void	rm_flag(uint64_t flag)
{
	singleton()->opts &= ~flag;
}

void	add_flag(uint64_t flag)
{
	singleton()->opts |= flag;
}

/*
** free this node which is a list: (t_list *)(singleton()->nodes->content)
*/
void	ft_free_node(void *lst_node)
{
	// t_list	*lst = (t_list *)lst_node;
	// t_list	*tmp = lst;
	// t_node	*node;

	// while (tmp)
	// {
	// 	// node = (t_node *)tmp->content;

	// 	// do a recursive trick here
	// 	// ft_lstclear(&node->recursive_nodes, NULL);
	// 	tmp = tmp->content;
	// }
	// ft_lstclear(&lst, NULL);
	(void)lst_node;
}

void	ft_free_all(void)
{
	if (singleton())
	{
		if (singleton()->args)
			ft_lstclear(&singleton()->args, NULL);
		if (singleton()->nodes)
			ft_lstclear(&singleton()->nodes, ft_free_node);
		free(singleton());
	}
}

t_ls	*singleton(void)
{
	static t_ls		*s = NULL;

	if (!s)
	{
		s = (t_ls *)ft_calloc(1, sizeof(t_ls));
		if (!s)
			return (NULL);
	}
	return (s);
}
