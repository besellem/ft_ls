/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_color.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/27 15:21:49 by besellem          #+#    #+#             */
/*   Updated: 2021/07/20 11:30:14 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	print_color(t_node *node)
{
	if (S_ISLNK(node->_lstats_.st_mode))
		ft_buffadd(PURPLE);
	else if (DT_DIR != node->_dir_.d_type && node->_lstats_.st_mode & S_IXOTH)
		ft_buffadd(RED);
	else if (DT_DIR == node->_dir_.d_type)
		ft_buffadd(B_CYAN);
}
