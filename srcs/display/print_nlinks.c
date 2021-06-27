/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_nlinks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/27 15:22:02 by besellem          #+#    #+#             */
/*   Updated: 2021/06/27 15:23:53 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	print_nlinks(t_node *node, t_padding *pads)
{
	char	*tmp = NULL;

	ft_asprintf(&tmp, "%*lld", pads->nlink, node->_stats_.st_nlink);
	ft_add2buf(tmp);
	ft_memdel((void **)&tmp);
	ft_add_char2buf(' ');
}
