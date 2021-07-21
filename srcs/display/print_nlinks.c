/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_nlinks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/27 15:22:02 by besellem          #+#    #+#             */
/*   Updated: 2021/07/21 11:56:52 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	print_nlinks(t_node *node, t_padding *pads)
{
	char	*tmp = NULL;

	ft_asprintf(&tmp, "%*lld", pads->nlink, node->_stats_.st_nlink);
	if (!tmp)
		ft_free_exit(EXIT_FAILURE, ERR_MSG_MALLOC);
	ft_buffadd(tmp);
	ft_memdel((void **)&tmp);
	ft_buffaddc(' ');
}
