/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_blocks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/27 15:21:43 by besellem          #+#    #+#             */
/*   Updated: 2022/04/12 11:09:19 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	print_blocks(const t_node *node, const t_pad *pads)
{
	char	*tmp = NULL;

	ft_asprintf(&tmp, "%*lld", pads->blocks, node->_stats_.st_blocks);
	if (!tmp)
		ft_free_exit();
	ft_buffadd(tmp);
	ft_memdel((void **)&tmp);
	ft_buffaddc(' ');
}
