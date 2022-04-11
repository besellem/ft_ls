/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_permissions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/27 15:20:13 by besellem          #+#    #+#             */
/*   Updated: 2022/04/11 22:52:11 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

inline static char	__get_mode__(mode_t mode)
{
	if (S_ISBLK(mode))  return ('b'); /* block special */
	if (S_ISCHR(mode))  return ('c'); /* char special */
	if (S_ISDIR(mode))  return ('d'); /* directory */
	if (S_ISLNK(mode))  return ('l'); /* symbolic link */
	if (S_ISFIFO(mode)) return ('p'); /* fifo or socket */
	if (S_ISSOCK(mode)) return ('s'); /* socket */
	return ('-');                     /* regular file by default */
}

void	print_permissions(const t_node *node)
{
	const mode_t	mode = is_flag(OPT_L) ? node->_stats_.st_mode : node->_lstats_.st_mode;
	char			*contructed_path = NULL;
	ssize_t			sd;

	ft_buffaddc(__get_mode__(mode));
	
	// user
	ft_buffaddc((mode & S_IRUSR) ? 'r' : '-');
	ft_buffaddc((mode & S_IWUSR) ? 'w' : '-');
	// ft_buffaddc((mode & S_IXUSR) ? 'x' : '-');
	ft_buffaddc((mode & S_IXUSR) ?
		((S_ISUID & mode) ? 's' : 'x') :
		((S_ISUID & mode) ? 'S' : '-'));

	// group
	ft_buffaddc((mode & S_IRGRP) ? 'r' : '-');
	ft_buffaddc((mode & S_IWGRP) ? 'w' : '-');
	ft_buffaddc((mode & S_IXGRP) ?
		((S_ISGID & mode) ? 's' : 'x') :
		((S_ISGID & mode) ? 'S' : '-'));

	// other
	ft_buffaddc((mode & S_IROTH) ? 'r' : '-');
	ft_buffaddc((mode & S_IWOTH) ? 'w' : '-');
	ft_buffaddc((S_IXOTH & mode) ?
		((S_ISVTX & mode) ? 't' : 'x') :
		((S_ISVTX & mode) ? 'T' : '-'));
	
	
	ft_asprintf(&contructed_path, "%s/%s", node->path, node->_dir_.d_name);
	if (!contructed_path)
		die();

	sd = listxattr(contructed_path, NULL, 0, XATTR_NOFOLLOW);
	ft_memdel((void **)&contructed_path);
	if (ENOTSUP == errno)
	{
		ft_buffaddc(' ');
		errno = 0;
	}
	else
	{
		if (sd <= 0)
			ft_buffadd("  ");
		else
			ft_buffadd("@ ");
	}
}
