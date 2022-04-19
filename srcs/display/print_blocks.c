/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_blocks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/27 15:21:43 by besellem          #+#    #+#             */
/*   Updated: 2022/04/19 12:44:41 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	print_blocks(const t_node *node, const t_pad *pads)
{
	char	*tmp = NULL;

#ifdef __linux__
	ft_asprintf(&tmp, "%*ld", pads->blocks, node->_stats_.st_blocks >> 1);
#else
	ft_asprintf(&tmp, "%*lld", pads->blocks, node->_stats_.st_blocks);
#endif
	if (!tmp)
		ft_free_exit();
	ft_buffadd(tmp);
	ft_memdel((void **)&tmp);
	ft_buffaddc(' ');
}
