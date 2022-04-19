/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_owner.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/27 15:22:05 by besellem          #+#    #+#             */
/*   Updated: 2022/04/19 12:13:07 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

#ifdef __linux__
# define SPACE_FMT
#else
# define SPACE_FMT " "
#endif

void	print_owner(const t_node *node, const t_pad *pads)
{
	struct passwd	*password = getpwuid(node->_stats_.st_uid);
	char			*tmp = NULL;
	
	if (!password)
		ft_free_exit();

	ft_asprintf(&tmp, "%-*s " SPACE_FMT, pads->owner_name, password->pw_name);
	if (!tmp)
		ft_free_exit();
	ft_buffadd(tmp);
	ft_memdel((void **)&tmp);
}
