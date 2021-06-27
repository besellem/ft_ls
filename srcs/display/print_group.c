/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_group.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/27 15:21:59 by besellem          #+#    #+#             */
/*   Updated: 2021/06/27 15:24:20 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	print_group(t_node *node, t_padding *pads)
{
	struct group	*grp = getgrgid(node->_stats_.st_gid);
	char			*tmp = NULL;

	ft_asprintf(&tmp, "%-*s", pads->group_name, grp->gr_name);
	ft_add2buf(tmp);
	ft_memdel((void **)&tmp);
	ft_add2buf("  ");
}
