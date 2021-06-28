/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 21:37:45 by besellem          #+#    #+#             */
/*   Updated: 2021/06/28 21:12:36 by besellem         ###   ########.fr       */
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


t_list	*ft_ls_file2lst(t_list **lst, char *path)
{
	t_list			*tmp;
	t_node			*node;

	node = (t_node *)ft_calloc(1, sizeof(t_node));

	/* need that to print its name */
	ft_bzero(&node->_dir_, sizeof(struct dirent));
	ft_memcpy(node->_dir_.d_name, path, ft_strlen(path));

	/* copy `struct stat' */
	stat(path, &node->_stats_);
	lstat(path, &node->_lstats_);

	/* add that node to the list */
	tmp = ft_lstnew(node);
	if (!tmp)
	{
		merror();
		ft_free_all();
		exit(EXIT_FAILURE);
	}
	ft_lstadd_back(lst, tmp);
	return (*lst);
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
		return (NULL);
	while ((s_dir = readdir(dir)))
	{
		node = (t_node *)ft_calloc(1, sizeof(t_node));

		/* keep current path */
		node->path = ft_strdup(path);

		/* Add node's name to the current path */
		ft_asprintf(&pwd, "%s/%s", path, s_dir->d_name);

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
			is_flag(OPT_R_MAJ) &&				/* `-R' option is set */
			ft_strcmp(s_dir->d_name, "..") &&	/* the current node is not the parent dir (avoid inf loop) */
			ft_strcmp(s_dir->d_name, "."))		/* the current node is not the current dir (avoid inf loop) */
		{
			ft_ls2lst(&node->recursive_nodes, pwd);
		}
		ft_memdel((void **)&pwd);

		/* add that node to the list */
		tmp = ft_lstnew(node);
		if (!tmp)
		{
			merror();
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

		if (ft_is_dir((char *)tmp->content))
			ft_ls2lst(&node_list, (char *)tmp->content);
		else
			ft_ls_file2lst(&node_list, (char *)tmp->content);
		
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

int	main(int ac, char **av)
{
	/* init singleton */
	if (NULL == singleton())
	{
		merror();
		return (EXIT_FAILURE);
	}

	/* parse arguments */
	if (ERR_CODE == parse_args(ac, av, &singleton()->args))
	{
		merror();
		ft_free_all();
		return (EXIT_FAILURE);
	}

	/* get all nodes asked by the args and the options set */
	singleton()->nodes = get_nodes(singleton()->args);
	if (NULL == singleton()->nodes)
	{
		// merror();
		ft_free_all();
		return (EXIT_FAILURE);
	}

	ft_print_entries(singleton()->nodes);
	
	ft_flush_buf();
	ft_free_all();
	return (EXIT_SUCCESS);
}
