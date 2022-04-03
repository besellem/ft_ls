/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 22:00:14 by besellem          #+#    #+#             */
/*   Updated: 2022/04/03 17:39:03 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int		is_flag(uint64_t flag)
{
	return ((singleton()->opts & flag) != 0);
}

void	rm_flag(uint64_t flag)
{
	singleton()->opts &= ~flag;
}

void	add_flag(uint64_t flag)
{
	singleton()->opts |= flag;
}
