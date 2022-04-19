/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_inode.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 16:26:50 by besellem          #+#    #+#             */
/*   Updated: 2022/04/19 12:11:54 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

#ifdef __linux__
# define ULONG_FMT "lu"
#else
# define ULONG_FMT "llu"
#endif

void	print_inode(const t_node *node, const t_pad *pads)
{
	char	*tmp = NULL;

	ft_asprintf(&tmp, "%*" ULONG_FMT " ", pads->inode, node->_stats_.st_ino);
	if (!tmp)
		die();
	ft_buffadd(tmp);	
	ft_memdel((void **)&tmp);
}
