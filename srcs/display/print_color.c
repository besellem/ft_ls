/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_color.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/27 15:21:49 by besellem          #+#    #+#             */
/*   Updated: 2022/04/13 12:13:48 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	print_color(const t_node *node)
{
	const mode_t	mode = node->_lstats_.st_mode;
	const bool		is_exec = (S_IXGRP & mode || S_IXUSR & mode || S_IXOTH & mode);
	// const bool		is_dir = ft_is_dir(node->path);
	const bool		is_dir = S_ISDIR(node->_stats_.st_mode);

	if (S_ISLNK(mode))
		ft_buffadd(is_flag(OPT_L) ? B_CYAN : PURPLE);
	else if (S_ISSOCK(mode))
		ft_buffadd(GREEN);
	else if S_ISFIFO(mode)
		ft_buffadd(YELLOW);
	else if (S_ISCHR(mode))
		ft_buffadd("\033[0;43;34m"); // BLUE TEXT + YELLOW FOREGROUND
	else if (S_ISBLK(mode))
		ft_buffadd("\033[0;46;34m"); // BLUE TEXT + CYAN FOREGROUND
	else if (is_dir)
		ft_buffadd((S_IWOTH & mode) ? "\033[0;43;30m" : B_CYAN);
	else if ((S_ISUID & mode) && is_exec) // setuid mode (chmod 4000)
		ft_buffadd("\033[0;41;30m"); // BLACK TEXT + RED FOREGROUND
	else if ((S_ISGID & mode) && is_exec) // setgid mode (chmod 2000)
		ft_buffadd("\033[0;47;30m"); // BLACK TEXT + GREY FOREGROUND
	else if (!is_dir && is_exec)
		ft_buffadd(RED);
}
