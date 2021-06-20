/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 22:51:45 by besellem          #+#    #+#             */
/*   Updated: 2021/06/20 23:18:14 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void	__add_char2buf__(char c)
{
	if (singleton()->buf_idx < _LS_BUFSIZ_)
		singleton()->buffer[singleton()->buf_idx++] = c;
	if (_LS_BUFSIZ_ == singleton()->buf_idx)
	{
		write(STDOUT_FILENO, singleton()->buffer, _LS_BUFSIZ_);
		singleton()->buf_idx = 0;
		ft_memset(singleton()->buffer, 0, _LS_BUFSIZ_);
	}
}

/*
** Instead of multiple call to `write', fill a buffer, and when it's full print
** it. Way faster (beating the real `ls' command).
*/
void	ft_add2buf(char *str)
{
	size_t	i;

	if (!str)
		return ;
	i = 0;
	while (str[i])
		__add_char2buf__(str[i++]);
}

void	ft_flush_buf(void)
{
	write(STDOUT_FILENO, singleton()->buffer, singleton()->buf_idx);
	ft_memset(singleton()->buffer, 0, singleton()->buf_idx);
}
