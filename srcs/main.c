/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 21:37:45 by besellem          #+#    #+#             */
/*   Updated: 2021/06/17 19:10:15 by besellem         ###   ########.fr       */
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

void	ft_lstprint(t_list *lst)
{
	t_list	*tmp = lst;

	while (tmp)
	{
		ft_printf("[%s]\n", (char *)tmp->content);
		tmp = tmp->next;
	}
}

// void	__recurse__(t_list *lst)
// {
// 	DIR				*dir;
// 	struct dirent	*s_dir;
// 	t_list			*tmp = lst;

// 	while (tmp)
// 	{
// 		dir = opendir((const char *)lst->content);
// 		while ((s_dir = readdir(dir)))
// 		{
// 			struct stat	st;
// 			stat(s_dir->d_name, &st);
// 			if (!is_flag(OPT_A_MIN) && (0 == ft_strcmp(s_dir->d_name, "..") ||
// 				0 == ft_strcmp(s_dir->d_name, ".")))
// 				continue ;
// 			print_dirent(s_dir);
// 			if (is_flag(OPT_S_MIN))
// 			{
// 				ft_printf("%d ", st.st_blocks);
// 			}
// 			ft_printf("%s\n", s_dir->d_name);
// 		}
// 		closedir(dir);
// 		tmp = tmp->next;
// 	}
// }


int	cmp_node_by_asc_time(t_node *n1, t_node *n2)
{
	return (n1->_stats_->st_mtimespec.tv_nsec > n2->_stats_->st_mtimespec.tv_nsec);
}

int	cmp_node_by_desc_time(t_node *n1, t_node *n2)
{
	return (n1->_stats_->st_mtimespec.tv_nsec < n2->_stats_->st_mtimespec.tv_nsec);
}

int	cmp_node_by_asc(t_node *n1, t_node *n2)
{
	return (ft_strcmp(n1->_dir_->d_name, n2->_dir_->d_name));
}

int	cmp_node_by_desc(t_node *n1, t_node *n2)
{
	return (ft_strcmp(n2->_dir_->d_name, n1->_dir_->d_name));
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
		if (is_flag(OPT_T_MAJ))
			ft_lst_sort(head, &cmp_node_by_asc_time);
		else
			ft_lst_sort(head, &cmp_node_by_asc);
	}
	else
	{
		if (is_flag(OPT_T_MAJ))
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
t_list	*ft_ls2lst(char *path)
{
	DIR				*dir = opendir(path);
	t_list			*lst = NULL;
	t_list			*tmp;
	t_node			node;
	struct dirent	*s_dir;

	// ERR()

	ft_printf("path: [%s], DIR *d: [%p]\n", path, dir);
	if (!dir)
		return (NULL);
	while ((s_dir = readdir(dir)))
	{
		ft_bzero(&node, sizeof(t_node));
		// ft_bzero(node._dir_, sizeof(struct dirent));
		// ft_bzero(node._stats_, sizeof(struct stat));
		// ft_bzero(node._lstats_, sizeof(struct stat));
		// ft_bzero(node.recursive_nodes, sizeof(t_list));

		node._dir_ = (struct dirent *)ft_calloc(1, sizeof(struct dirent));
		ft_memmove(node._dir_, s_dir, sizeof(struct dirent));

		// ft_printf("len: [%d]\n", node._dir_->d_namlen);

		/* get stat struct */
		node._stats_ = (struct stat *)ft_calloc(1, sizeof(struct stat));
		stat(s_dir->d_name, node._stats_);
		
		node._lstats_ = (struct stat *)ft_calloc(1, sizeof(struct stat));
		lstat(s_dir->d_name, node._lstats_);

		// ft_printf("[%s]\n", s_dir->d_name);
		
		/* if it's a directory & the flag `-R' is set, make a recursive call */
		// if (DT_DIR == s_dir->d_type && is_flag(OPT_R_MAJ))
		// 	node.recursive_nodes = ft_ls2lst(s_dir->d_name);
		
		/* add that node to the list */
		tmp = ft_lstnew(&node);
		if (!tmp)
		{
			ERR()
			ft_free_all();
			exit(EXIT_FAILURE);
		}
		ft_lstadd_back(&lst, tmp);
	}
	closedir(dir);
	ft_sort_lst_nodes(&lst);
	return (lst);
}

/*
** 
*/
t_list	*get_nodes(t_list *args)
{
	t_list	*nodes = NULL;
	t_list	*tmp = args;
	t_list	*new_tmp;

	/* if the list of args is empty, do `ls' on the current dir */
	if (EMPTY == ft_lstsize(args))
	{
		new_tmp = ft_ls2lst(".");
		if (!new_tmp)
		{
			ft_lstclear(&nodes, ft_free_node);
			return (NULL);
		}
		ft_lstadd_front(&nodes, new_tmp);
	}
	else
	{
		while (tmp)
		{
			new_tmp = ft_ls2lst(tmp->content);
			if (!new_tmp)
			{
				ft_lstclear(&nodes, ft_free_node);
				return (NULL);
			}
			ft_lstadd_back(&nodes, new_tmp);
			tmp = tmp->content;
		}
	}
	// (t_node *)(singleton()->nodes->content);
	return (nodes);
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

void	ft_do_ls(t_list	*lst)
{
	(void)lst;
}

void	__print_lst__(void)
{
	t_list	*lst = singleton()->nodes;
	t_list	*tmp;
	t_node	*node;

	while (lst)
	{
		tmp = (t_list *)lst->content;
		node = (t_node *)(tmp->content);
		// printf("ptr [%p]\n", &node);
		ft_printf("ptr: [%p]\n", node->_dir_);
		// write(1, node->_dir_.d_name, node->_dir_.d_namlen - 1);
		// ft_printf("[%s]\n", node->_dir_.d_name);
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
		return (EXIT_FAILURE);
	}
	
	// ft_lstprint(singleton()->args);

	singleton()->nodes = get_nodes(singleton()->args);
	// ft_lstclear(&singleton()->args, NULL);

	__print_lst__();

	// ft_printf("flag [%.64b]\n\n", singleton()->opts);
	// if (EMPTY == ft_lstsize(singleton()->args))
	// 	ft_ls(".");
	// else

	// __recurse__(singleton()->args);
	// ft_ls();
	ft_free_all();
	return (EXIT_SUCCESS);
}

/*



*/