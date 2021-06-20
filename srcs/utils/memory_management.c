/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 22:32:46 by besellem          #+#    #+#             */
/*   Updated: 2021/06/20 22:33:28 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void	__free_lst__(t_list *head)
{
	t_list	*lst = head;
	t_node	*node;

	while (lst)
	{
		node = (t_node *)lst->content;
		if (node)
		{
			if (node->recursive_nodes)
				__free_lst__(node->recursive_nodes);
		}
		ft_memdel((void **)&node);
		lst = lst->next;
	}
	ft_lstclear(&head, NULL);
}

static void	ft_free_nodes(t_list *head)
{
	t_list	*lst = head;

	while (lst)
	{
		__free_lst__((t_list *)lst->content);
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
