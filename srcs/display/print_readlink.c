/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_readlink.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/27 15:22:10 by besellem          #+#    #+#             */
/*   Updated: 2022/04/03 17:24:21 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	print_readlink(t_node *node)
{
	char	buf[FILENAME_MAX + 1] = {0};
	char	*filename;

	ft_asprintf(&filename, "%s/%s", node->path, node->_dir_.d_name);
	if (!filename)
		ft_free_exit();
	readlink(filename, buf, FILENAME_MAX);
	ft_memdel((void **)&filename);
	ft_buffadd(" -> ");
	ft_buffadd(buf);
}
