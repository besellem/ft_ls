/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 22:22:52 by besellem          #+#    #+#             */
/*   Updated: 2021/06/15 22:32:52 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	ft_free_all(void)
{
	if (singleton())
	{
		if (singleton()->lst)
			ft_lstclear(&singleton()->lst, NULL);
		free(singleton());
	}
}

t_ls	*singleton(void)
{
	static t_ls	*s = NULL;

	if (!s)
	{
		s = (t_ls *)ft_calloc(1, sizeof(t_ls));
		if (!s)
			return (NULL);
	}
	return (s);
}
