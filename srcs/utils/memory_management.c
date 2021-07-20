/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 22:32:46 by besellem          #+#    #+#             */
/*   Updated: 2021/07/20 17:51:26 by besellem         ###   ########.fr       */
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
			ft_memdel((void **)&node->path);
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
	t_buffer_attr	attr = {.fd = STDOUT_FILENO, .oflag = BUF_INIT};

	ft_init_buff(&attr); // release buffer memory in case of a malloc error during print
	if (singleton())
	{
		if (singleton()->args)
			ft_lstclear(&singleton()->args, NULL);
		if (singleton()->nodes)
			ft_free_nodes(singleton()->nodes);
		free(singleton());
	}
}

const char	*g_error_msgs[] = {
	[ERR_MSG_MALLOC] = "malloc error"
};

void	ft_free_exit(int val, enum e_error_msg __error)
{
	if ((int)__error != NO_ERR)
		ft_printf(PROG_NAME ": %s\n", g_error_msgs[__error]);
	ft_free_all();
	exit(val);
}
