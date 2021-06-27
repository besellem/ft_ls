/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_blocks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/27 15:21:43 by besellem          #+#    #+#             */
/*   Updated: 2021/06/27 15:23:36 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	print_blocks(t_node *node, t_padding *pads)
{
	char	*tmp = NULL;

	ft_asprintf(&tmp, "%*lld", pads->blocks, node->_stats_.st_blocks);
	ft_add2buf(tmp);
	ft_memdel((void **)&tmp);
	ft_add_char2buf(' ');
}
