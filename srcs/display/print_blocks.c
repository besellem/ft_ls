/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_blocks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/27 15:21:43 by besellem          #+#    #+#             */
/*   Updated: 2022/04/06 01:46:25 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

// TODO: to fix for links with `-L' option
// try `ls -lGL ~/.config/iterm2'
void	print_blocks(const t_node *node, const t_pad *pads)
{
	// const struct stat	_stat = is_flag(OPT_L) ? node->_stats_ : node->_lstats_;
	char				*tmp = NULL;

	ft_asprintf(&tmp, "%*lld", pads->blocks, node->_stats_.st_blocks);
	if (!tmp)
		ft_free_exit();
	ft_buffadd(tmp);
	ft_memdel((void **)&tmp);
	ft_buffaddc(' ');
}
