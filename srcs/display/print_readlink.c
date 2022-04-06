/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_readlink.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/27 15:22:10 by besellem          #+#    #+#             */
/*   Updated: 2022/04/06 01:59:01 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	print_readlink(const t_node *node)
{
	char	buf[FILENAME_MAX + 1] = {0};
	char	*filename;

	if (ft_strcmp(node->path, node->_dir_.d_name) == 0)
		filename = ft_strdup(node->path);
	else
		ft_asprintf(&filename, "%s/%s", node->path, node->_dir_.d_name);
	
	ft_printf("[%s]\n", filename);
	if (!filename)
		ft_free_exit();
	readlink(filename, buf, FILENAME_MAX);
	ft_memdel((void **)&filename);
	ft_buffadd(" -> ");
	ft_buffadd(buf);
}
