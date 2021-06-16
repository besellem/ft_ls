/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 21:37:45 by besellem          #+#    #+#             */
/*   Updated: 2021/06/16 17:59:42 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	print_dirent(struct dirent *d)
{
	ft_printf("{\n");
	ft_printf("    dirent->d_ino     [%llu]\n",  d->d_ino);
	ft_printf("    dirent->d_name    [%s]\n",    d->d_name);
	ft_printf("    dirent->d_namlen  [%d]\n",    d->d_namlen);
	ft_printf("    dirent->d_reclen  [%d]\n",    d->d_reclen);
	ft_printf("    dirent->d_seekoff [%#llx]\n", d->d_seekoff);
	ft_printf("    dirent->d_type    [%c]\n",    d->d_type);
	ft_printf("}\n");
}

void	print_stat(struct stat *d)
{
	ft_printf("{\n");
	ft_printf("    dirent->d_ino     [%ld]\n",   d->st_atimespec.tv_nsec);
	ft_printf("    dirent->d_ino     [%ld]\n",   d->st_atimespec.tv_sec);
	ft_printf("    dirent->d_ino     [%ld]\n",   d->st_birthtimespec.tv_nsec);
	ft_printf("    dirent->d_ino     [%ld]\n",   d->st_birthtimespec.tv_nsec);
	ft_printf("    dirent->d_ino     [%llu]\n",  d->st_blksize);
	ft_printf("    dirent->d_ino     [%llu]\n",  d->st_atimespec);
	ft_printf("    dirent->d_ino     [%llu]\n",  d->st_atimespec);
	ft_printf("    dirent->d_ino     [%llu]\n",  d->st_atimespec);
	ft_printf("    dirent->d_ino     [%llu]\n",  d->st_atimespec);
	ft_printf("    dirent->d_ino     [%llu]\n",  d->st_atimespec);
	ft_printf("    dirent->d_ino     [%llu]\n",  d->st_atimespec);
	ft_printf("    dirent->d_ino     [%llu]\n",  d->st_atimespec);
	ft_printf("    dirent->d_ino     [%llu]\n",  d->st_atimespec);
	ft_printf("    dirent->d_ino     [%llu]\n",  d->st_atimespec);
	ft_printf("}\n");
}

void	__recurse__(t_list *lst)
{
	DIR				*dir;
	struct dirent	*s_dir;
	t_list			*tmp = lst;

	while (tmp)
	{
		dir = opendir((const char *)lst->content);
		while ((s_dir = readdir(dir)))
		{
			struct stat	st;
			stat(s_dir->d_name, &st);
			if (!is_flag(OPT_A_MIN) && (0 == ft_strcmp(s_dir->d_name, "..") ||
				0 == ft_strcmp(s_dir->d_name, ".")))
				continue ;
			print_dirent(s_dir);
			if (is_flag(OPT_S_MIN))
			{
				ft_printf("%d ", st.st_blocks);
			}
			ft_printf("%s\n", s_dir->d_name);
		}
		closedir(dir);
		tmp = tmp->next;
	}
}

void	ft_lstprint(t_list *lst)
{
	t_list	*tmp = lst;

	while (tmp)
	{
		ft_printf("[%s]\n", (char *)tmp->content);
		tmp = tmp->next;
	}
}

int	main(int ac, const char **av)
{
	if (NULL == singleton())
		return (EXIT_FAILURE);
	if (ERR_CODE == parse_args(ac, av))
	{
		ft_free_all();
		return (EXIT_FAILURE);
	}
	// ft_printf("flag [%.64b]\n\n", singleton()->opts);
	// ft_lstprint(singleton()->lst);
	__recurse__(singleton()->lst);
	// ft_ls();
	ft_free_all();
	return (EXIT_SUCCESS);
}
