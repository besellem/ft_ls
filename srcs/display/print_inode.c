/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_inode.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 16:26:50 by besellem          #+#    #+#             */
/*   Updated: 2022/04/08 17:26:58 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	print_inode(const t_node *node, const t_pad *pads)
{
	char	*tmp = NULL;

	ft_asprintf(&tmp, "%*llu ", pads->inode, node->_stats_.st_ino);
	if (!tmp)
		die();
	ft_buffadd(tmp);	
	ft_memdel((void **)&tmp);
}
