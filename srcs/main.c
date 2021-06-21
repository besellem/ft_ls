/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 21:37:45 by besellem          #+#    #+#             */
/*   Updated: 2021/06/22 00:21:47 by besellem         ###   ########.fr       */
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
	ft_printf("    stat->st_atimespec.tv_nsec     [%ld]\n",  d->st_atimespec.tv_nsec);
	ft_printf("    stat->st_atimespec.tv_sec      [%ld]\n",  d->st_atimespec.tv_sec);
	ft_printf("    stat->st_birthtimespec.tv_nsec [%ld]\n",  d->st_birthtimespec.tv_nsec);
	ft_printf("    stat->st_birthtimespec.tv_nsec [%ld]\n",  d->st_birthtimespec.tv_nsec);
	ft_printf("    stat->st_blksize               [%d]\n",   d->st_blksize);
	ft_printf("    stat->st_blocks                [%lld]\n", d->st_blocks);
	ft_printf("    stat->st_ctimespec.tv_nsec     [%ld]\n",  d->st_ctimespec.tv_nsec);
	ft_printf("    stat->st_ctimespec.tv_sec      [%ld]\n",  d->st_ctimespec.tv_sec);
	ft_printf("    stat->st_dev                   [%d]\n",   d->st_dev);
	ft_printf("    stat->st_flags                 [%u]\n",   d->st_flags);
	ft_printf("    stat->st_gen                   [%u]\n",   d->st_gen);
	ft_printf("    stat->st_gid                   [%u]\n",   d->st_gid);
	ft_printf("    stat->st_ino                   [%llu]\n", d->st_ino);
	ft_printf("    stat->st_mode                  [%d]\n",   d->st_mode);
	ft_printf("    stat->st_mtimespec.tv_nsec     [%ld]\n",  d->st_mtimespec.tv_nsec);
	ft_printf("    stat->st_mtimespec.tv_sec      [%ld]\n",  d->st_mtimespec.tv_sec);
	ft_printf("    stat->st_nlink                 [%u]\n",   d->st_nlink);
	ft_printf("    stat->st_rdev                  [%d]\n",   d->st_rdev);
	ft_printf("    stat->st_size                  [%lld]\n", d->st_size);
	ft_printf("    stat->st_uid                   [%u]\n",   d->st_uid);
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

/*
** `lst->content' is a `t_node *' structure.
** Each `t_node *' structure contains the infos about the file / folder.
** If the `-R' option is enable, we add a list to that node until no more
** directories are found in that path.
*/
t_list	*ft_ls2lst(t_list **lst, char *path)
{
	DIR				*dir = opendir(path);
	t_list			*tmp;
	t_node			*node;
	char			*pwd = NULL;
	struct dirent	*s_dir;

	if (!dir)
	{
		ft_printf(PROG_NAME ": %s: %s\n", path, strerror(errno));
		return (NULL);
	}
	while ((s_dir = readdir(dir)))
	{
		node = (t_node *)ft_calloc(1, sizeof(t_node));

		/* Add node's name to the current path */
		ft_asprintf(&pwd, "%s/%s", path, s_dir->d_name);
		
		// ft_printf("s_dir[%p] node[%p] s_dir->d_name[%s]\n",
		// 	s_dir, node, s_dir->d_name);

		/* copy that `struct dirent' */
		ft_memcpy(&node->_dir_, s_dir, sizeof(struct dirent));

		/* copy `struct stat' */
		/*
		** OPTI -> call `stat' and `lstat' only when the options need that infos
		*/
		stat(pwd, &node->_stats_);
		lstat(pwd, &node->_lstats_);

		// print_stat(&node->_stats_);
		// print_stat(&node->_lstats_);

		/* if it's a directory & the flag `-R' is set, make a recursive call */
		if (DT_DIR == s_dir->d_type && is_flag(OPT_R_MAJ) &&
			ft_strcmp(s_dir->d_name, "..") && ft_strcmp(s_dir->d_name, "."))
		{
			ft_ls2lst(&node->recursive_nodes, pwd);
		}
		ft_memdel((void **)&pwd);

		/* add that node to the list */
		tmp = ft_lstnew(node);
		if (!tmp)
		{
			ft_free_all();
			exit(EXIT_FAILURE);
		}
		ft_lstadd_back(lst, tmp);
	}
	closedir(dir);
	ft_sort_lst_nodes(lst);
	return (*lst);
}

/*
** 
*/
t_list	*get_nodes(t_list *args)
{
	t_list	*nodes = NULL;			/* main list */
	t_list	*new_node = NULL;		/* list of nodes containing */
	t_list	*node_list;				/*  */
	t_list	*tmp = args;

	while (tmp)
	{
		/* `t_list' containing all the lists from a path (which is an argument) */
		node_list = NULL;
		ft_ls2lst(&node_list, (char *)tmp->content);
		
		/* when the directory passed in args does exist */
		if (node_list)
		{
			/* `t_list' containing `node_list' above */
			new_node = ft_lstnew(node_list);
			
			/* append the new list to the main one */
			ft_lstadd_back(&nodes, new_node);
		}
		tmp = tmp->next;
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
	// ft_printf("mode[%16b] ", node->_stats_.st_mode);
	ft_add_char2buf(get_mode(node->_stats_.st_mode));
	ft_add_char2buf((node->_stats_.st_mode & S_IRUSR) ? 'r' : '-');
	ft_add_char2buf((node->_stats_.st_mode & S_IWUSR) ? 'w' : '-');
	ft_add_char2buf((node->_stats_.st_mode & S_IXUSR) ? 'x' : '-');
	ft_add_char2buf((node->_stats_.st_mode & S_IRGRP) ? 'r' : '-');
	ft_add_char2buf((node->_stats_.st_mode & S_IWGRP) ? 'w' : '-');
	ft_add_char2buf((node->_stats_.st_mode & S_IXGRP) ? 'x' : '-');
	ft_add_char2buf((node->_stats_.st_mode & S_IROTH) ? 'r' : '-');
	ft_add_char2buf((node->_stats_.st_mode & S_IWOTH) ? 'w' : '-');
	ft_add_char2buf((node->_stats_.st_mode & S_IXOTH) ? 'x' : '-');
	ft_add_char2buf(' ');
}

void	print_color(t_node *node)
{
	if (DT_DIR == node->_dir_.d_type)
		ft_add2buf(B_CYAN);
}

void	ft_print_entry(t_list *current, t_node *node)
{
	char	*tmp = NULL;

	/* if `-A' is set & the node's name starts is either `.' or `..', do not print that node */
	if (is_flag(OPT_A_MAJ) && !is_flag(OPT_A_MIN) &&
		(0 == ft_strcmp(node->_dir_.d_name, "..") ||
		0 == ft_strcmp(node->_dir_.d_name, ".")))
		return ;

	/* if `-a' is not set & the node's name starts with a `.', do not print that node */
	if (!is_flag(OPT_A_MAJ) && !is_flag(OPT_A_MIN) &&
		0 == ft_strncmp(node->_dir_.d_name, ".", 1))
		return ;
	
	/* print nbr of blocks if `-s' is set */
	if (is_flag(OPT_S_MIN))
	{
		/*
		** OPTI -> get the padding before the call instead of recalculating it for each node
		*/
		ft_asprintf(&tmp, "%*lld ", s_padding(current), node->_stats_.st_blocks);
		ft_add2buf(tmp);
		ft_memdel((void **)&tmp);
	}
	
	/* print permissions if `-l' (ell) is set */
	if (is_flag(OPT_L_MIN))
	{
		print_permissions(node);
	}

	/*
	** print node's name
	*/
	/* option `-G' turns on the colors */
	if (is_flag(OPT_G_MAJ))
		print_color(node);
	
	/* print entry name */
	ft_add2buf(node->_dir_.d_name);

	if (is_flag(OPT_G_MAJ))
		ft_add2buf(CLR_COLOR);

	/* add `/' when the option `-p' is turned on and if it's a directory */
	if (is_flag(OPT_P_MIN) && DT_DIR == node->_dir_.d_type)
		ft_add_char2buf('/');
	
	/* new line -- end of the entry */
	ft_add_char2buf('\n');
}

static void	__print_lst_recursively__(t_list *head)
{
	t_list	*lst;
	t_node	*node;

	/* first print all the current list */
	lst = head;
	while (lst)
	{
		node = (t_node *)lst->content;
		if (node)
			ft_print_entry((t_list *)head, node);
		lst = lst->next;
	}
	
	/* then print the recursive lists */
	lst = head;
	while (lst)
	{
		node = (t_node *)lst->content;
		if (node && is_flag(OPT_R_MAJ) && node->recursive_nodes)
			__print_lst_recursively__(node->recursive_nodes);
		lst = lst->next;
	}
}

void	__print_entries__(t_list *head)
{
	t_list	*lst = head;

	while (lst)
	{
		__print_lst_recursively__((t_list *)lst->content);
		lst = lst->next;
	}
}

int	main(int ac, const char **av)
{
	/* init singleton */
	if (NULL == singleton())
	{
		ERR()
		return (EXIT_FAILURE);
	}

	/* parse arguments */
	if (ERR_CODE == parse_args(ac, av, &singleton()->args))
	{
		ft_free_all();
		ERR()
		return (EXIT_FAILURE);
	}

	/* get all nodes asked by the args and the options set */
	singleton()->nodes = get_nodes(singleton()->args);
	if (NULL == singleton()->nodes)
	{
		ft_free_all();
		return (EXIT_FAILURE);
	}

	__print_entries__(singleton()->nodes);
	
	ft_flush_buf();
	ft_free_all();
	return (EXIT_SUCCESS);
}
