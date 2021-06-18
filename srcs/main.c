/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 21:37:45 by besellem          #+#    #+#             */
/*   Updated: 2021/06/18 16:54:46 by besellem         ###   ########.fr       */
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
	ft_printf("    dirent->d_ino     [%ld]\n",  d->st_atimespec.tv_nsec);
	ft_printf("    dirent->d_ino     [%ld]\n",  d->st_atimespec.tv_sec);
	ft_printf("    dirent->d_ino     [%ld]\n",  d->st_birthtimespec.tv_nsec);
	ft_printf("    dirent->d_ino     [%ld]\n",  d->st_birthtimespec.tv_nsec);
	ft_printf("    dirent->d_ino     [%d]\n",   d->st_blksize);
	ft_printf("    dirent->d_ino     [%lld]\n", d->st_blocks);
	ft_printf("    dirent->d_ino     [%ld]\n",  d->st_ctimespec.tv_nsec);
	ft_printf("    dirent->d_ino     [%ld]\n",  d->st_ctimespec.tv_sec);
	ft_printf("    dirent->d_ino     [%d]\n",   d->st_dev);
	ft_printf("    dirent->d_ino     [%u]\n",   d->st_flags);
	ft_printf("    dirent->d_ino     [%u]\n",   d->st_gen);
	ft_printf("    dirent->d_ino     [%u]\n",   d->st_gid);
	ft_printf("    dirent->d_ino     [%llu]\n", d->st_ino);
	ft_printf("    dirent->d_ino     [%d]\n",   d->st_lspare);
	ft_printf("    dirent->d_ino     [%d]\n",   d->st_mode);
	ft_printf("    dirent->d_ino     [%ld]\n",  d->st_mtimespec.tv_nsec);
	ft_printf("    dirent->d_ino     [%ld]\n",  d->st_mtimespec.tv_sec);
	ft_printf("    dirent->d_ino     [%u]\n",   d->st_nlink);
	ft_printf("    dirent->d_ino     [%lld]\n", d->st_qspare);
	ft_printf("    dirent->d_ino     [%d]\n",   d->st_rdev);
	ft_printf("    dirent->d_ino     [%lld]\n", d->st_size);
	ft_printf("    dirent->d_ino     [%u]\n",   d->st_uid);
	ft_printf("}\n");
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

void	ft_lstprint_ptr(t_list *lst)
{
	t_list	*tmp = lst;

	ft_printf("listsize: [" B_GREEN "%d" CLR_COLOR "]\n", ft_lstsize(lst));
	while (tmp)
	{
		ft_printf("[" B_RED "%p" CLR_COLOR "] [" B_RED "%p" CLR_COLOR "]\n", tmp, tmp->next);
		tmp = tmp->next;
	}
}

int	cmp_node_by_asc_time(t_node *n1, t_node *n2)
{
	const time_t	n1_sec = n1->_stats_.st_mtimespec.tv_sec;
	const time_t	n2_sec = n2->_stats_.st_mtimespec.tv_sec;
	
	if (n1_sec == n2_sec)
		return (n1->_stats_.st_mtimespec.tv_nsec > n2->_stats_.st_mtimespec.tv_nsec);
	return (n1_sec > n2_sec);
}

int	cmp_node_by_desc_time(t_node *n1, t_node *n2)
{
	const time_t	n1_sec = n1->_stats_.st_mtimespec.tv_sec;
	const time_t	n2_sec = n2->_stats_.st_mtimespec.tv_sec;
	
	if (n1_sec == n2_sec)
		return (n1->_stats_.st_mtimespec.tv_nsec < n2->_stats_.st_mtimespec.tv_nsec);
	return (n1_sec < n2_sec);
}

int	cmp_node_by_asc(t_node *n1, t_node *n2)
{
	return (ft_strcmp(n2->_dir_.d_name, n1->_dir_.d_name));
}

int	cmp_node_by_desc(t_node *n1, t_node *n2)
{
	return (ft_strcmp(n1->_dir_.d_name, n2->_dir_.d_name));
}

/*
** Sort a list
*/
void	ft_lst_sort(t_list **lst, int (*cmp)())
{
	t_list	*tmp;
	void	*content;

	tmp = *lst;
	while (tmp && tmp->next)
	{
		if (cmp(tmp->content, tmp->next->content) > 0)
		{
			// ERR()
			content = tmp->content;
			tmp->content = tmp->next->content;
			tmp->next->content = content;
			tmp = *lst;
		}
		else
			tmp = tmp->next;
	}
}

void	ft_sort_lst_nodes(t_list **head)
{
	if (is_flag(OPT_R_MIN))
	{
		if (is_flag(OPT_T_MIN))
			ft_lst_sort(head, &cmp_node_by_asc_time);
		else
			ft_lst_sort(head, &cmp_node_by_asc);
	}
	else
	{
		if (is_flag(OPT_T_MIN))
			ft_lst_sort(head, &cmp_node_by_desc_time);
		else
			ft_lst_sort(head, &cmp_node_by_desc);
	}
}

/*
** t_list	*lst;
** `lst->content' is a `t_node *' structure.
** Each node contains all infos about the file / folder.
** If the `-R' option is enable, we add a list to that node till we don't find
** any directories anymore in that path.
*/
t_list	*ft_ls2lst(t_list *lst, char *path)
{
	DIR				*dir = opendir(path);
	// t_list			*lst = NULL;
	t_list			*tmp;
	t_node			*node;
	char			*pwd;
	struct dirent	*s_dir;

	if (!dir)
	{
		ft_printf(PROG_NAME ": %s: %s\n", path, strerror(errno));
		return (NULL);
	}
	// if (!lst || !path)
	// 	return (NULL);
	while ((s_dir = readdir(dir)))
	{
		node = (t_node *)ft_calloc(1, sizeof(t_node));
		// ft_bzero(&node->_dir_, sizeof(struct dirent));	// may be faster without
		// ft_bzero(&node->_stats_, sizeof(struct stat));	// may be faster without
		// ft_bzero(&node->_lstats_, sizeof(struct stat));	// may be faster without

		/* copy that `struct dirent' */
		ft_memmove(&node->_dir_, s_dir, sizeof(struct dirent));

		/* copy `struct stat' */
		stat(s_dir->d_name, &node->_stats_);
		lstat(s_dir->d_name, &node->_lstats_);
		
		/* if it's a directory & the flag `-R' is set, make a recursive call */
		if (DT_DIR == s_dir->d_type && is_flag(OPT_R_MAJ) &&
			ft_strcmp(s_dir->d_name, "..") && ft_strcmp(s_dir->d_name, "."))
		{
			/* Add the folder's name to the current path to search from */
			ft_asprintf(&pwd, "%s/%s", path, s_dir->d_name);
			node->recursive_nodes = ft_ls2lst(node->recursive_nodes, pwd);
			ft_memdel((void **)&pwd);
		}
		
		/* add that node to the list */
		tmp = ft_lstnew(node);
		if (!tmp)
		{
			ERR()
			ft_free_all();
			exit(EXIT_FAILURE);
		}
		ft_lstadd_back(&lst, tmp);
	}
	closedir(dir);
	ft_lstprint_ptr(lst);
	// ft_sort_lst_nodes(&lst);
	return (lst);
}

/*
** 
*/
t_list	*get_nodes(t_list *args)
{
	t_list	*nodes = NULL;
	t_list	*lst;
	t_list	*tmp;
	t_list	*new_tmp;

	/* if the list of args is empty, do `ls' on the current dir */
	if (EMPTY == ft_lstsize(args))
	{
		lst = ft_lstnew((t_list *)ft_calloc(1, sizeof(t_list)));
		new_tmp = ft_lstnew(ft_ls2lst(lst, "."));
		if (!lst || !new_tmp)
		{
			ft_lstclear(&nodes, NULL);
			return (NULL);
		}
		ft_lstadd_front(&nodes, new_tmp);
	}
	else
	{
		tmp = args;
		while (tmp)
		{
			lst = ft_lstnew((t_list *)ft_calloc(1, sizeof(t_list)));
			new_tmp = ft_lstnew(ft_ls2lst(lst, tmp->content));
			
			/* when the directory passed in args does not exist */
			if (lst && new_tmp)
			{
				// lst = ft_lstnew(new_tmp);
				ft_lstadd_back(&nodes, new_tmp);
			}
			// else
			// 	free(lst);
			tmp = tmp->next;
		}
	}
	return (nodes);
}

int	s_padding(t_list *lst)
{
	t_list	*tmp = lst;
	t_node	*node;
	int		padding;
	int		len;

	padding = 0;
	while (tmp)
	{
		node = (t_node *)tmp->content;
		if (node)
		{
			len = ft_nblen(node->_stats_.st_blocks);
			if (len > padding)
				padding = len;
		}
		tmp = tmp->next;
	}
	return (padding);
}

char	get_mode(mode_t mode)
{
	if (S_ISBLK(mode))	return ('b'); /* block special */
	if (S_ISCHR(mode))	return ('c'); /* char special */
	if (S_ISDIR(mode))	return ('d'); /* directory */
	if (S_ISREG(mode))	return ('-'); /* regular file */
	if (S_ISLNK(mode))	return ('l'); /* symbolic link */
	if (S_ISSOCK(mode))	return ('s'); /* socket */
	if (S_ISFIFO(mode))	return ('p'); /* fifo or socket */
	return ('-');
}

void	print_permissions(t_node *node)
{
	ft_printf("mode[%16b] ", node->_stats_.st_mode);
	ft_putchar(get_mode(node->_stats_.st_mode));
	ft_putchar((node->_stats_.st_mode & S_IRUSR) ? 'r' : '-');
	ft_putchar((node->_stats_.st_mode & S_IWUSR) ? 'w' : '-');
	ft_putchar((node->_stats_.st_mode & S_IXUSR) ? 'x' : '-');
	ft_putchar((node->_stats_.st_mode & S_IRGRP) ? 'r' : '-');
	ft_putchar((node->_stats_.st_mode & S_IWGRP) ? 'w' : '-');
	ft_putchar((node->_stats_.st_mode & S_IXGRP) ? 'x' : '-');
	ft_putchar((node->_stats_.st_mode & S_IROTH) ? 'r' : '-');
	ft_putchar((node->_stats_.st_mode & S_IWOTH) ? 'w' : '-');
	ft_putchar((node->_stats_.st_mode & S_IXOTH) ? 'x' : '-');
	ft_putchar(' ');
}

void	ft_print_entry(t_list *current, t_node *node)
{
	if (!is_flag(OPT_A_MIN) && 0 == ft_strncmp(node->_dir_.d_name, ".", 1))
		return ;
	if (is_flag(OPT_S_MIN))
	{
		// ft_printf("%d ",   node->_stats_.st_blksize);
		ft_printf("%*lld ", s_padding(current), node->_stats_.st_blocks);
	}
	
	if (is_flag(OPT_L_MIN))
	{
		print_permissions(node);
	}

	/*
	** print node's name
	*/
	/* option `-G' turns on the colors */
	if (is_flag(OPT_G_MAJ) && DT_DIR == node->_dir_.d_type)
		ft_putstr(B_CYAN);
	
	/* print entry name */
	ft_printf("%s" CLR_COLOR, node->_dir_.d_name);

	/* add `/' when the option `-p' is turned on and if it's a directory */
	if (is_flag(OPT_P_MIN) && DT_DIR == node->_dir_.d_type)
		ft_putstr("/");
	
	/* new line -- end of the entry */
	ft_putstr("\n");
}

// void	ft_ls(char *path)
// {
// 	DIR				*dir;
// 	struct dirent	*s_dir;
// 	struct stat		st;

// 	dir = opendir(path);
// 	while ((s_dir = readdir(dir)))
// 	{
// 		stat(s_dir->d_name, &st);
// 		if (!is_flag(OPT_A_MIN) && (0 == ft_strcmp(s_dir->d_name, "..") ||
// 			0 == ft_strncmp(s_dir->d_name, ".", 1)))
// 			continue ;
// 		// print_dirent(s_dir);
// 		if (is_flag(OPT_S_MIN))
// 		{
// 			ft_printf("%d ", st.st_blocks);
// 		}
// 		ft_printf("%s\n", s_dir->d_name);
// 	}
// 	closedir(dir);
// }

void	__print_lst__(t_list *head)
{
	t_list	*lst = head;
	t_list	*tmp;
	t_node	*node;

	while (lst)
	{
		tmp = (t_list *)lst->content;
		while (tmp)
		{
			node = (t_node *)tmp->content;
			if (node)
			{
				ft_print_entry((t_list *)lst->content, node);
				if (is_flag(OPT_R_MAJ) && node->recursive_nodes)
					__print_lst__(node->recursive_nodes);
			}
			tmp = tmp->next;
		}
		lst = lst->next;
	}
}

int	main(int ac, const char **av)
{
	if (NULL == singleton())
		return (EXIT_FAILURE);
	if (ERR_CODE == parse_args(ac, av))
	{
		ft_free_all();
		ERR()
		return (EXIT_FAILURE);
	}

	singleton()->nodes = get_nodes(singleton()->args);
	if (NULL == singleton()->nodes)
	{
		ft_free_all();
		ERR()
		return (EXIT_FAILURE);
	}
	
	// ft_lstclear(&singleton()->args, NULL);

	ft_printf("main list size: %d\n", ft_lstsize(singleton()->nodes));
	ft_lstprint_ptr(singleton()->nodes);

	ft_printf("\n----------------\n\n");

	// t_list	*__tmp__ = singleton()->nodes;
	// for (; __tmp__ ; __tmp__ = __tmp__->next)
	// {
	// 	ft_printf("lstsize: %d\n", ft_lstsize(__tmp__->content));
	// 	ft_lstprint_ptr(__tmp__->content);
	// }

	ft_printf("\n----------------\n\n");

	__print_lst__(singleton()->nodes);

	// ft_printf("flag [%.64b]\n\n", singleton()->opts);

	// __recurse__(singleton()->args);
	// ft_ls();
	// ft_free_all();
	return (EXIT_SUCCESS);
}
