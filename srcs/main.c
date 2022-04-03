/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 21:37:45 by besellem          #+#    #+#             */
/*   Updated: 2022/04/03 17:35:18 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"


////////////////////////////////////////////////////////////////////////////////
// DEBUG PURPOSE ONLY
////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////
// END DEBUG PURPOSE ONLY
////////////////////////////////////////////////////////////////////////////////



t_list	*ft_ls_file2lst(t_list *lst, char *path)
{
	t_node	*node;

	node = (t_node *)ft_calloc(1, sizeof(t_node));
	if (!node)
		ft_free_exit();

	/* need that to print its name */
	ft_bzero(&node->_dir_, sizeof(struct dirent));
	ft_memcpy(node->_dir_.d_name, path, ft_strlen(path));

	/* copy `struct stat' */
	stat(path, &node->_stats_);
	lstat(path, &node->_lstats_);

	/* add that node to the list */
	if (!ft_lst_push_back(&lst, node))
		ft_free_exit();
	return (lst);
}

/*
** `lst->content' is a `t_node *' structure.
** Each `t_node *' structure contains the infos about the file / folder.
** If the `-R' option is enable, we add a list to that node until no more
** directories are found in that path.
*/
void	ft_ls2lst(t_list **lst, char *path)
{
	DIR				*dir = opendir(path);
	t_node			*node;
	char			*pwd = NULL;
	struct dirent	*s_dir;

	if (!dir)
		return ;
	while ((s_dir = readdir(dir)))
	{
		node = (t_node *)ft_calloc(1, sizeof(t_node));
		if (!node)
			ft_free_exit();

		/* keep current path */
		node->path = ft_strdup(path);
		if (!node->path)
			ft_free_exit();

		/* Add node's name to the current path */
		ft_asprintf(&pwd, "%s/%s", path, s_dir->d_name);
		if (!pwd)
			ft_free_exit();

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
		if (DT_DIR == s_dir->d_type &&			/* is a directory*/
			is_flag(OPT_R) &&					/* `-R' option is set */
			ft_strcmp(s_dir->d_name, "..") &&	/* the current node is not the parent dir (avoid inf loop) */
			ft_strcmp(s_dir->d_name, "."))		/* the current node is not the current dir (avoid inf loop) */
		{
			ft_ls2lst(&node->recursive_nodes, pwd);
		}
		ft_memdel((void **)&pwd);

		/* add that node to the list */
		if (!ft_lst_push_back(lst, node))
			ft_free_exit();
	}
	closedir(dir);
	ft_sort_lst_nodes(lst);
	return ;
}

/*
** 
*/
t_list	*get_nodes(t_list *args)
{
	t_list	*nodes = NULL;	/* main list */
	t_list	*node_list;		/*  */

	while (args)
	{
		/* `t_list' containing all the lists from a path (which is an argument) */
		node_list = NULL;
		ft_printf("- [%s]\n", (char *)args->content);

		if (ft_is_dir((char *)args->content))
			ft_ls2lst(&node_list, (char *)args->content);
		else
			ft_ls_file2lst(node_list, (char *)args->content);
		
		if (!node_list)
			ft_free_exit();
		
		/* when the directory passed in args does exist */
		if (node_list)
		{
			/* append the new list to the main one */
			if (!ft_lst_push_back(&nodes, node_list))
				ft_free_exit();
		}
		args = args->next;
	}
	return (nodes);
}

int		main(int ac, char **av)
{
	/* init singleton */
	if (NULL == singleton())
		ft_free_exit();

	/* parse arguments */
	if (FALSE == ft_parse_args(ac, av, &singleton()->args))
		ft_free_exit();

	/* get all nodes asked by the args and the options set */
	singleton()->nodes = get_nodes(singleton()->args);
	if (NULL == singleton()->nodes)
		ft_free_exit();

	// ft_printf("%p\n", singleton()->nodes);
	ft_print_entries(singleton()->nodes);
	
	ft_flush_buff();
	ft_free_all();
	return (EXIT_SUCCESS);
}
