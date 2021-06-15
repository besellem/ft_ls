/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 21:37:45 by besellem          #+#    #+#             */
/*   Updated: 2021/06/15 22:48:27 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	__recurse__(const char **av)
{
	(void)av;
}

void	ft_lstprint(t_list *lst)
{
	t_list	*tmp = lst;

	while (tmp)
	{
		ft_printf("[%s]\n", (char *)tmp->content);
		tmp = tmp->next;
	}
}

int	main(int ac, const char **av)
{
	if (NULL == singleton())
		return (EXIT_FAILURE);
	if (ERR_CODE == parse_args(ac, av))
	{
		ft_free_all();
		return (EXIT_FAILURE);
	}
	ft_lstprint(singleton()->lst);
	// __recurse__(ac);
	// ft_ls();
	ft_printf("flag [%.64b]\n", singleton()->opts);
	ft_free_all();
	return (EXIT_SUCCESS);
}
