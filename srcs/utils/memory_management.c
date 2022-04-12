/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 22:32:46 by besellem          #+#    #+#             */
/*   Updated: 2022/04/12 10:43:58 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	rm_arg(t_args **head, t_args *to_rm)
{
	t_args	*tmp = *head;
	t_args	*to_free = NULL;

	if (!*head || !to_rm)
	{
		free(to_rm);
		return ;
	}
	
	if (*head == to_rm)
	{
		to_free = *head;
		*head = (*head)->next;
	}
	else
	{
		while (tmp && tmp->next && tmp->next != to_rm)
			tmp = tmp->next;
		to_free = tmp->next;
		tmp->next = tmp->next->next;
	}
	free(to_free);
}

void	free_node_lst(node_list_t *head)
{
	t_node	*node;

	for (node_list_t *tmp = head; tmp != NULL; tmp = tmp->next)
	{
		node = tmp->content;
		
		if (node)
		{
			/*
			** node->constructed_path may be a reference to node->path.
			** In this case, only node->path must be freed.
			*/
			if (node->path != node->constructed_path)
				ft_memdel((void **)&node->constructed_path);
			ft_memdel((void **)&node->path);
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
