/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_inode.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 16:26:50 by besellem          #+#    #+#             */
/*   Updated: 2022/04/08 16:40:38 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	print_inode(const t_node *node, const t_pad *pads)
{
	const struct stat	_stat = is_flag(OPT_L) ? node->_stats_ : node->_lstats_;
	char				*tmp = NULL;

	ft_asprintf(&tmp, "%*llu ", pads->inode, _stat.st_ino);
	if (!tmp)
		die();
	ft_buffadd(tmp);	
	ft_memdel((void **)&tmp);
}
