/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 21:37:45 by besellem          #+#    #+#             */
/*   Updated: 2022/04/15 18:45:22 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void	__ls_on_file__(const char *path)
{
	node_list_t	*lst = NULL;
	t_node		*node = alloc_node();

	node->path = ft_strdup(path);
	node->constructed_path = node->path;
	if (!node->path)
		die();
	
	/* needed to print its name */
	ft_strncpy(node->_dir_.d_name, path, PATH_MAX);

	/* copy file's infos into the node */
	stat(path, &node->_stats_);
	lstat(path, &node->_lstats_);

	/* add that node to the list */
	lst_push_back(&lst, node);

	__print_entries_lst__(lst, false, false);
	free_node_lst(lst);
}

/*
** `lst->content' is a `t_node *' structure.
** Each `t_node *' structure contains the infos about the file / folder.
** If the `-R' option is enable, we add a list to that node until no more
** directories are found in that path.
*/
static void	__ls_on_dir__(const char *path, bool print_header)
{
	node_list_t		*lst = NULL;
	DIR				*dir = opendir(path);
	t_node			*node;
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
		if (STRISEQ(node->path, node->_dir_.d_name))
			node->constructed_path = node->path;
		else
			ft_asprintf(&node->constructed_path, "%s/%s", path, s_dir->d_name);
		if (!node->constructed_path)
			die();

		/* copy element's infos into the node */
		ft_memcpy(&node->_dir_, s_dir, sizeof(*s_dir));
		stat(node->constructed_path, &node->_stats_);
		lstat(node->constructed_path, &node->_lstats_);

		/* add that node to the list */
		lst_push_sorted(&lst, node, get_cmp_method());
	}
	closedir(dir);
	dir = NULL;


	/* print this dir entries */
	__print_entries_lst__(lst, print_header, true);


	/* no need to go further if there's no dive to do */
	if (!is_flag(OPT_R))
		return (void)free_node_lst(lst);


	/* search for recursive directories to print */
	for (node_list_t *rec_node = lst; rec_node; rec_node = rec_node->next)
	{
		node = rec_node->content;
		
		/* if it's a directory & the flag `-R' is set, make a recursive call */
		if (DT_DIR == node->_dir_.d_type &&			/* is a directory */
			ft_strcmp(node->_dir_.d_name, "..") &&	/* the current node is not the parent dir (avoid inf loop) */
			ft_strcmp(node->_dir_.d_name, "."))		/* the current node is not the current dir (avoid inf loop) */
		{
			if (0 == ft_strncmp(node->_dir_.d_name, ".", 1))
			{
				if (is_flag(OPT_A_MIN))
				{
					ft_buffaddc('\n');
					__ls_on_dir__(node->constructed_path, true);
				}
			}
			else
			{
				ft_buffaddc('\n');
				__ls_on_dir__(node->constructed_path, true);
			}
		}
	}

	free_node_lst(lst);
}


/*
** Parse all files and directories of requested path(s) (contained in `arguments')
*/
static void	do_ls(const char *prog_name, t_args **arguments)
{
	t_args			*arg = NULL;
	t_args			*to_del;
	char			*current_path;
	bool			is_dir;
	bool			listed_files = false;
	bool			listed_error = false;
	bool			print_header = false;
	struct stat		_s;


	/* ls on all files first */
	for (arg = *arguments; arg; )
	{
		current_path = arg->content->_dir_.d_name;
		is_dir = ft_is_dir(current_path);
		
		to_del = NULL;
		
		if (!is_dir || (S_ISLNK(arg->content->_lstats_.st_mode) &&
			!is_flag(OPT_L)) || is_flag(OPT_D_MIN))
		{
			if (SYSCALL_ERR == stat(current_path, &_s))
			{
				ft_dprintf(STDERR_FILENO, "%s: %s: %s\n",
					prog_name, current_path, strerror(errno));
				errno = 0;
				listed_error = true;
			}
			else
			{
				__ls_on_file__(current_path);
			}
			listed_files = true;
			to_del = arg;
		}
		arg = arg->next;
		rm_arg(arguments, to_del);
	}

	if (listed_files && !listed_error && *arguments)
		ft_buffaddc('\n');
	
	
	/* check if the path header must be printed */
	print_header = (lst_size(*arguments) > 1) || listed_files;

	/* ls on all folders */
	for (arg = *arguments; arg; arg = arg->next)
	{
		current_path = arg->content->_dir_.d_name;
		is_dir = ft_is_dir(current_path);
		
		if (is_dir)// && !is_flag(OPT_D_MIN))
		{
			__ls_on_dir__(current_path, print_header);
			if (arg->next)
				ft_buffaddc('\n');
		}
	}
}

int			main(int ac, char **av)
{
	/* parse arguments */
	if (FALSE == ft_parse_args(ac, av, &singleton()->args))
		die();

	do_ls(av[0], &singleton()->args);

	return (EXIT_SUCCESS);
}
