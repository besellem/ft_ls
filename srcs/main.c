/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 21:37:45 by besellem          #+#    #+#             */
/*   Updated: 2022/04/11 23:15:50 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void	__ls_on_file__(node_list_t **lst, const char *path)
{
	t_node	*node = alloc_node();

	node->path = ft_strdup(path);
	if (!node->path)
		die();

	/* need that to print its name */
	ft_strncpy(node->_dir_.d_name, path, PATH_MAX);

	/* copy `struct stat' */
	stat(path, &node->_stats_);
	lstat(path, &node->_lstats_);

	/* add that node to the list */
	lst_push_back(lst, node);

	__print_entries_lst__(*lst, true);
	__free_node_lst__(*lst);
	*lst = NULL;
}

/*
** `lst->content' is a `t_node *' structure.
** Each `t_node *' structure contains the infos about the file / folder.
** If the `-R' option is enable, we add a list to that node until no more
** directories are found in that path.
*/
static void	__ls_on_dir__(node_list_t **lst, const char *path)
{
	DIR				*dir = opendir(path);
	t_node			*node;
	char			*contructed_path = NULL;
	struct dirent	*s_dir = NULL;

	if (!dir)
		return ;

	/* add each file/folder of THIS `path' in a list of nodes */
	while ((s_dir = readdir(dir)))
	{
		node = alloc_node();

		/* keep current path */
		node->path = ft_strdup(path);
		if (!node->path)
			die();

		/* Add node's name to the current path */
		ft_asprintf(&contructed_path, "%s/%s", path, s_dir->d_name);
		if (!contructed_path)
			die();

		/* copy that `struct dirent' */
		ft_memcpy(&node->_dir_, s_dir, sizeof(*s_dir));

		/* copy `struct stat' */
		stat(contructed_path, &node->_stats_);
		lstat(contructed_path, &node->_lstats_);
		
		ft_memdel((void **)&contructed_path);

		/* add that node to the list */
		lst_push_sorted(lst, node, get_cmp_method());
	}
	closedir(dir);
	dir = NULL;


	/* print this dir entries */
	__print_entries_lst__(*lst, true);


	/* no need to go further if there's no dive to do */
	if (!is_flag(OPT_R))
	{
		__free_node_lst__(*lst);
		*lst = NULL;
		return ;
	}


	/* search for recursive directories to print */
	for (node_list_t *rec_node = *lst; rec_node; rec_node = rec_node->next)
	{
		node = rec_node->content;
		
		/* if it's a directory & the flag `-R' is set, make a recursive call */
		if (DT_DIR == node->_dir_.d_type &&			/* is a directory */
			ft_strcmp(node->_dir_.d_name, "..") &&	/* the current node is not the parent dir (avoid inf loop) */
			ft_strcmp(node->_dir_.d_name, "."))		/* the current node is not the current dir (avoid inf loop) */
		{
			/* Add node's name to the current path */
			ft_asprintf(&contructed_path, "%s/%s", path, node->_dir_.d_name);
			if (!contructed_path)
				die();

			if (0 == ft_strncmp(node->_dir_.d_name, ".", 1))
			{
				if (is_flag(OPT_A_MIN))
				{
					ft_buffaddc('\n');
					__ls_on_dir__(&node->recursive_nodes, contructed_path);
				}
			}
			else
			{
				ft_buffaddc('\n');
				__ls_on_dir__(&node->recursive_nodes, contructed_path);
			}
			ft_memdel((void **)&contructed_path);
		}
	}

	__free_node_lst__(*lst);
	*lst = NULL;
}

/*
** Parse all files and directories of requested path(s) (contained in `arguments')
*/
static void	do_ls(const t_args *arguments)
{
	node_list_t		*node_list;
	char			*current_path;
	bool			is_dir;
	// char			last_path_char;
	// struct stat		st;

	for (t_args *arg = (t_args *)arguments; arg; arg = arg->next)
	{
		current_path = arg->content->_dir_.d_name;
		
		/* `node_list_t' containing all the lists from a path (which is an argument) */
		node_list = NULL;

		// ft_bzero(&st, sizeof(st));
		// stat(current_path, &st);

		// last_path_char = current_path[ft_strlen(current_path) - 1];
		is_dir = ft_is_dir(current_path);
		
		// TODO: try `ls -lL /var'
		if (is_dir && !is_flag(OPT_D_MIN))// && (last_path_char == '/')) // diff btween /var/ & /var for example
		{
			__ls_on_dir__(&node_list, current_path);
		}
		else
		{
			__ls_on_file__(&node_list, current_path);
		}
	}
}

int			main(int ac, char **av)
{
	/* parse arguments */
	if (FALSE == ft_parse_args(ac, av, &singleton()->args))
		die();

	do_ls(singleton()->args);

	return (EXIT_SUCCESS);
}
