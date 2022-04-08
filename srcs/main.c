/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 21:37:45 by besellem          #+#    #+#             */
/*   Updated: 2022/04/08 18:04:40 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	ft_ls_file2lst(node_list_t **lst, const char *path)
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
}

/*
** `lst->content' is a `t_node *' structure.
** Each `t_node *' structure contains the infos about the file / folder.
** If the `-R' option is enable, we add a list to that node until no more
** directories are found in that path.
*/
void	ft_ls2lst(node_list_t **lst, const char *path)
{
	DIR				*dir = opendir(path);
	t_node			*node;
	char			*contructed_path = NULL;
	struct dirent	*s_dir;

	if (!dir)
		return ;

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
		lst_push_sorted(lst, node, get_cmp_method());
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
list_t	*get_nodes(const t_args *arguments)
{
	list_t			*nodes = NULL;
	node_list_t		*node_list;
	t_node			*_node;
	// char			last_path_char;
	char			*current_path = NULL;
	struct stat		_s = {0};

	for (t_args *arg = (t_args *)arguments; arg; arg = arg->next)
	{
		_node = arg->content;
		current_path = _node->_dir_.d_name;
		
		/* `node_list_t' containing all the lists from a path (which is an argument) */
		node_list = NULL;
		
		if (stat(current_path, &_s) < 0)
			die();

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
			die();
		
		/* when the directory passed in args does exist */
		if (node_list)
		{
			/* append the new list to the main one */
			lst_push_back(&nodes, node_list);
		}
	}
	return (nodes);
}

int		main(int ac, char **av)
{
	/* parse arguments */
	if (FALSE == ft_parse_args(ac, av, &singleton()->args))
		die();

	/* get all nodes asked by the args and the options set */
	singleton()->nodes = get_nodes(singleton()->args);
	if (NULL == singleton()->nodes)
		die();

	/* not needed anymore */
	lst_clear(&singleton()->args, free);
	
	__print_nodes__(singleton()->nodes);

	ft_flush_buff();

	return (EXIT_SUCCESS);
}
