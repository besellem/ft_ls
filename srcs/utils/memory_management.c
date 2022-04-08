/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 22:32:46 by besellem          #+#    #+#             */
/*   Updated: 2022/04/08 17:06:13 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	__free_node_lst__(node_list_t *head)
{
	t_node	*node;

	for (node_list_t *tmp = head; tmp != NULL; tmp = tmp->next)
	{
		node = (t_node *)tmp->content;
		
		if (node)
		{
			ft_memdel((void **)&node->path);
			__free_node_lst__(node->recursive_nodes);
		}
		ft_memdel((void **)&node);
	}
	lst_clear(&head, LST_CLEAR_IGN);
}

void	ft_free_nodes(list_t **head)
{
	for (list_t *lst = *head; lst != NULL; lst = lst->next)
	{
		__free_node_lst__(lst->content);
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
