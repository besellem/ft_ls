/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_group.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/27 15:21:59 by besellem          #+#    #+#             */
/*   Updated: 2022/04/06 01:37:30 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	print_group(const t_node *node, const t_pad *pads)
{
	struct group	*grp = getgrgid(node->_stats_.st_gid);
	char			*tmp = NULL;

	ft_asprintf(&tmp, "%-*s  ", pads->group_name, grp->gr_name);
	if (!tmp)
		ft_free_exit();
	ft_buffadd(tmp);
	ft_memdel((void **)&tmp);
}
