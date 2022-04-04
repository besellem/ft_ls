/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_permissions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/27 15:20:13 by besellem          #+#    #+#             */
/*   Updated: 2022/04/04 17:34:07 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

inline static char	__get_mode__(mode_t mode)
{
	if (S_ISBLK(mode))  return ('b'); /* block special */
	if (S_ISCHR(mode))  return ('c'); /* char special */
	if (S_ISDIR(mode))  return ('d'); /* directory */
	if (S_ISREG(mode))  return ('-'); /* regular file */
	if (S_ISLNK(mode))  return ('l'); /* symbolic link */
	if (S_ISSOCK(mode)) return ('s'); /* socket */
	if (S_ISFIFO(mode)) return ('p'); /* fifo or socket */
	return ('-');
}

void	print_permissions(const t_node *node)
{
	const mode_t	mode = node->_lstats_.st_mode;

	ft_buffaddc(__get_mode__(mode));
	
	// user
	ft_buffaddc((mode & S_IRUSR) ? 'r' : '-');
	ft_buffaddc((mode & S_IWUSR) ? 'w' : '-');
	ft_buffaddc((mode & S_IXUSR) ? 'x' : '-');
	
	// group
	ft_buffaddc((mode & S_IRGRP) ? 'r' : '-');
	ft_buffaddc((mode & S_IWGRP) ? 'w' : '-');
	ft_buffaddc((mode & S_IXGRP) ? 'x' : '-');
	
	// other
	ft_buffaddc((mode & S_IROTH) ? 'r' : '-');
	ft_buffaddc((mode & S_IWOTH) ? 'w' : '-');
	if (mode & S_ISVTX)
		ft_buffaddc((mode & S_IXOTH) ? 't' : 'T');
	else
		ft_buffaddc((mode & S_IXOTH) ? 'x' : '-');
	


	// ft_printf("mode [%hd]\n", mode);
	
	// char		*str = node->_dir_.d_name;
	// // char		*str = node->path;
	// ssize_t		sd = listxattr(str, NULL, 0, 0);
	
	// printf("[%s] %zd\n", str, sd);
	
	// if (sd > 0)
	// 	ft_buffadd("@ ");
	// else
	ft_buffadd("  ");
}
