/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_owner.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/27 15:22:05 by besellem          #+#    #+#             */
/*   Updated: 2021/07/20 11:30:14 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	print_owner(t_node *node, t_padding *pads)
{
	struct passwd	*password = getpwuid(node->_stats_.st_uid);
	char			*tmp = NULL;

	ft_asprintf(&tmp, "%-*s", pads->owner_name, password->pw_name);
	ft_buffadd(tmp);
	ft_memdel((void **)&tmp);
	ft_buffadd("  ");
}
