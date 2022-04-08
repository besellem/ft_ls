/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 22:32:46 by besellem          #+#    #+#             */
/*   Updated: 2022/04/08 15:19:49 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	__free_lst__(t_list *head)
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
	lst_clear(&head, LST_CLEAR_IGN);
}

void	ft_free_nodes(t_list **head)
{
	for (t_list *lst = *head; lst != NULL; lst = lst->next)
	{
		__free_lst__((t_list *)lst->content);
	}
	lst_clear(head, LST_CLEAR_IGN);
}

void	ft_free_all(void)
{
	ft_destroy_buff();
	if (singleton())
	{
		if (singleton()->args)
			lst_clear(&singleton()->args, free);
		if (singleton()->nodes)
			ft_free_nodes(&singleton()->nodes);
		free(singleton());
	}
}
