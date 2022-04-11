/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 22:32:46 by besellem          #+#    #+#             */
/*   Updated: 2022/04/11 09:29:30 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	__free_node_lst__(node_list_t *head)
{
	t_node	*node;

	for (node_list_t *tmp = head; tmp != NULL; tmp = tmp->next)
	{
		node = tmp->content;
		
		if (node)
		{
			ft_memdel((void **)&node->path);
			__free_node_lst__(node->recursive_nodes);
		}
		ft_memdel((void **)&node);
	}
	lst_clear(&head, LST_CLEAR_IGN);
}

__destructor void	ft_free_all(void)
{
	ft_flush_buff();
	if (singleton())
	{
		if (singleton()->args)
			lst_clear(&singleton()->args, free);
		free(singleton());
	}
}
