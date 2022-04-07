/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 21:37:45 by besellem          #+#    #+#             */
/*   Updated: 2022/04/07 15:43:37 by besellem         ###   ########.fr       */
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
////////////////////////////////////////////////////////////////////////////////
// END DEBUG PURPOSE ONLY
////////////////////////////////////////////////////////////////////////////////


void	ft_ls_file2lst(t_list **lst, const char *path)
{
	t_node	*node = alloc_node();

	if (!node)
		ft_free_exit();

	node->path = ft_strdup(path);
	if (!node->path)
		ft_free_exit();

	/* need that to print its name */
	ft_memcpy(node->_dir_.d_name, path, ft_strnlen(path, PATH_MAX));

	/* copy `struct stat' */
	stat(path, &node->_stats_);
	lstat(path, &node->_lstats_);

	/* add that node to the list */
	if (!ft_lst_push_back(lst, node))
		ft_free_exit();
}

/*
** `lst->content' is a `t_node *' structure.
** Each `t_node *' structure contains the infos about the file / folder.
** If the `-R' option is enable, we add a list to that node until no more
** directories are found in that path.
*/
void	ft_ls2lst(t_list **lst, const char *path)
{
	int				(*_cmp)() = get_cmp_method();
	DIR				*dir = opendir(path);
	t_node			*node;
	char			*contructed_path = NULL;
	struct dirent	*s_dir;

	if (!dir)
		return ;

	while ((s_dir = readdir(dir)))
	{
		node = alloc_node();
		if (!node)
			ft_free_exit();

		/* keep current path */
		node->path = ft_strdup(path);
		if (!node->path)
			ft_free_exit();

		/* Add node's name to the current path */
		ft_asprintf(&contructed_path, "%s/%s", path, s_dir->d_name);
		if (!contructed_path)
			ft_free_exit();

		/* copy that `struct dirent' */
		ft_memcpy(&node->_dir_, s_dir, sizeof(*s_dir));

		/* copy `struct stat' */
		stat(contructed_path, &node->_stats_);
		lstat(contructed_path, &node->_lstats_);

		// print_stat(&node->_stats_);
		// print_stat(&node->_lstats_);

		/* if it's a directory & the flag `-R' is set, make a recursive call */
		if (DT_DIR == s_dir->d_type &&			/* is a directory */
			is_flag(OPT_R) &&					/* `-R' option is set */
			ft_strcmp(s_dir->d_name, "..") &&	/* the current node is not the parent dir (avoid inf loop) */
			ft_strcmp(s_dir->d_name, "."))		/* the current node is not the current dir (avoid inf loop) */
		{
			if (0 == ft_strncmp(s_dir->d_name, ".", 1))
			{
				if (is_flag(OPT_A_MIN))
					ft_ls2lst(&node->recursive_nodes, contructed_path);
			}
			else
				ft_ls2lst(&node->recursive_nodes, contructed_path);
			
			// ft_ls2lst(&node->recursive_nodes, contructed_path);
		}
		ft_memdel((void **)&contructed_path);

		/* add that node to the list */
		ft_lst_push_sorted(lst, node, _cmp);
	}
	closedir(dir);
	
	// ft_buffadd("\n[]\n");
	// __print_lst__(*lst, true);
	// ft_buffaddc('\n');
	// __free_lst__(*lst);
	
	return ;
}

/*
** Parse all files and directories of requested path(s) (contained in `arguments')
*/
t_list	*get_nodes(const t_list *arguments)
{
	t_list			*nodes = NULL;	/* main list */
	t_list			*node_list;		/*  */
	t_node			*_arg_node;
	// char			last_path_char;
	char			*current_path = NULL;
	struct stat		_s = {0};

	for (t_list *arg = (t_list *)arguments; arg; arg = arg->next)
	{
		_arg_node = (t_node *)arg->content;
		current_path = _arg_node->_dir_.d_name;
		
		/* `t_list' containing all the lists from a path (which is an argument) */
		node_list = NULL;
		
		if (stat(current_path, &_s) < 0)
			ft_free_exit();

		// last_path_char = current_path[ft_strlen(current_path) - 1];

		// TODO: try `ls -lL /var'
		if (ft_is_dir(current_path))// && (last_path_char == '/')) // diff btween /var/ & /var for example
		{
			ft_ls2lst(&node_list, current_path);
		}
		else
		{
			ft_ls_file2lst(&node_list, current_path);
		}
		
		if (!node_list)
			ft_free_exit();
		
		/* when the directory passed in args does exist */
		if (node_list)
		{
			/* append the new list to the main one */
			if (!ft_lst_push_back(&nodes, node_list))
				ft_free_exit();
		}
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

	// ft_lst_print(singleton()->args, ft_putendl);

	/* get all nodes asked by the args and the options set */
	singleton()->nodes = get_nodes(singleton()->args);
	if (NULL == singleton()->nodes)
		ft_free_exit();

	/* not needed anymore */
	ft_free_nodes(&singleton()->args);
	
	__print_nodes__(singleton()->nodes);

	ft_flush_buff();
	ft_free_all();

	return (EXIT_SUCCESS);
}
