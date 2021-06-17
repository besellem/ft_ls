/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 22:22:52 by besellem          #+#    #+#             */
/*   Updated: 2021/06/17 23:56:25 by besellem         ###   ########.fr       */
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

void	ft_free_nodes(t_list *head)
{
	t_list	*lst = head;
	t_node	*node;

	while (lst)
	{
		node = (t_node *)lst->content;
		if (node)
		{
			if (node->recursive_nodes)
				ft_free_nodes(node->recursive_nodes);
		}
		ft_memdel((void **)&node);
		lst = lst->next;
	}
	ft_lstclear(&head, NULL);
}

void	ft_free_all(void)
{
	if (singleton())
	{
		if (singleton()->args)
			ft_lstclear(&singleton()->args, NULL);
		if (singleton()->nodes)
			ft_free_nodes(singleton()->nodes);
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
