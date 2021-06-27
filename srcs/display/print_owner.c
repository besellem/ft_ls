/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_owner.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/27 15:22:05 by besellem          #+#    #+#             */
/*   Updated: 2021/06/27 15:24:07 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	print_owner(t_node *node, t_padding *pads)
{
	struct passwd	*password = getpwuid(node->_stats_.st_uid);
	char			*tmp = NULL;

	ft_asprintf(&tmp, "%-*s", pads->owner_name, password->pw_name);
	ft_add2buf(tmp);
	ft_memdel((void **)&tmp);
	ft_add2buf("  ");
}
