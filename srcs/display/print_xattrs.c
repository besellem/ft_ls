/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_xattrs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 15:35:46 by besellem          #+#    #+#             */
/*   Updated: 2022/04/07 16:25:54 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	print_xattrs(const t_node *node)
{
	char		*tmp = NULL;
	char		*contructed_path = NULL;
	char		buf[PATH_MAX] = {0};
	char		val[PATH_MAX];
	ssize_t		list_len = 0;
	ssize_t		attr_val;

	ft_asprintf(&contructed_path, "%s/%s", node->path, node->_dir_.d_name);
	if (!contructed_path)
		ft_free_exit();

	list_len = listxattr(contructed_path, buf, sizeof(buf), XATTR_NOFOLLOW);
	if (list_len > 0)
	{
		for (ssize_t i = 0; i < list_len; i += ft_strlen(&buf[i]) + 1)
		{
			ft_bzero(val, sizeof(val));
			attr_val = getxattr(contructed_path, &buf[i], val, PATH_MAX, 0, XATTR_NOFOLLOW);
			
			if (attr_val < 0)
				continue ;

			ft_asprintf(&tmp, "\t%s\t   %zd \n", &buf[i], attr_val);
			if (!tmp)
				ft_free_exit();
			ft_buffadd(tmp);
			ft_memdel((void **)&tmp);
		}
	}
	ft_memdel((void **)&contructed_path);
}
