/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_nlinks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/27 15:22:02 by besellem          #+#    #+#             */
/*   Updated: 2022/04/12 10:05:14 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	print_nlinks(const t_node *node, const t_pad *pads)
{
	const struct stat	_stat = is_flag(OPT_L) ? node->_stats_ : node->_lstats_;
	char				*tmp = NULL;

	ft_asprintf(&tmp, "%*hu ", pads->nlink, _stat.st_nlink);
	if (!tmp)
		ft_free_exit();
	ft_buffadd(tmp);
	ft_memdel((void **)&tmp);
}
