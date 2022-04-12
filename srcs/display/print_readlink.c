/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_readlink.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/27 15:22:10 by besellem          #+#    #+#             */
/*   Updated: 2022/04/12 10:22:57 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	print_readlink(const t_node *node)
{
	char	buf[FILENAME_MAX + 1] = {0};

	readlink(node->constructed_path, buf, FILENAME_MAX);
	ft_buffadd(" -> ");
	ft_buffadd(buf);
}
