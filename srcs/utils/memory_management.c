/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 22:32:46 by besellem          #+#    #+#             */
/*   Updated: 2022/04/04 15:03:01 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void	__free_lst__(t_list *head)
{
	t_node	*node;

	for (t_list *tmp = head; tmp != NULL; tmp = tmp->next)
	{
		node = (t_node *)tmp->content;
		
		if (node)
		{
			ft_memdel((void **)&node->path);
			__free_lst__(node->recursive_nodes);
		}
		ft_memdel((void **)&node);
	}
	ft_lstclear(&head, NULL);
}

static void	ft_free_nodes(t_list *head)
{
	for (t_list *lst = head; lst != NULL; lst = lst->next)
	{
		__free_lst__((t_list *)lst->content);
	}
	ft_lstclear(&head, NULL);
}

void	ft_free_all(void)
{
	ft_destroy_buff();
	if (singleton())
	{
		if (singleton()->args)
			ft_lstclear(&singleton()->args, NULL);
		if (singleton()->nodes)
			ft_free_nodes(singleton()->nodes);
		free(singleton());
	}
}
