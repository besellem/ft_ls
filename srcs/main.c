/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 21:37:45 by besellem          #+#    #+#             */
/*   Updated: 2021/06/24 18:40:11 by besellem         ###   ########.fr       */
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

		/* Add node's name to the current path */
		ft_asprintf(&pwd, "%s/%s", path, s_dir->d_name);
		
		// ft_printf("s_dir[%p] node[%p] s_dir->d_name[%s]\n",
		// 	s_dir, node, s_dir->d_name);

		node->path = ft_strdup(path);

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
			ft_free_all();
			exit(EXIT_FAILURE);
		}
		ft_lstadd_back(lst, tmp);
	}
	closedir(dir);
	ft_sort_lst_nodes(lst);
	return (*lst);
}


int	ft_is_dir(char *path)
{
	DIR	*d = opendir(path);

	if (!d)
		return (FALSE);
	closedir(d);
	return (TRUE);
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

int	s_padding(t_list *lst)
{
	int		padding = 0;
	t_list	*tmp = lst;
	t_node	*node;
	int		tmp_len;

	while (tmp)
	{
		node = (t_node *)tmp->content;
		if (node)
		{
			tmp_len = ft_nblen(node->_stats_.st_blocks);
			if (tmp_len > padding)
				padding = tmp_len;
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
	ft_add_char2buf(get_mode(node->_lstats_.st_mode));
	ft_add_char2buf((node->_lstats_.st_mode & S_IRUSR) ? 'r' : '-');
	ft_add_char2buf((node->_lstats_.st_mode & S_IWUSR) ? 'w' : '-');
	ft_add_char2buf((node->_lstats_.st_mode & S_IXUSR) ? 'x' : '-');
	ft_add_char2buf((node->_lstats_.st_mode & S_IRGRP) ? 'r' : '-');
	ft_add_char2buf((node->_lstats_.st_mode & S_IWGRP) ? 'w' : '-');
	ft_add_char2buf((node->_lstats_.st_mode & S_IXGRP) ? 'x' : '-');
	ft_add_char2buf((node->_lstats_.st_mode & S_IROTH) ? 'r' : '-');
	ft_add_char2buf((node->_lstats_.st_mode & S_IWOTH) ? 'w' : '-');
	ft_add_char2buf((node->_lstats_.st_mode & S_IXOTH) ? 'x' : '-');
	ft_add_char2buf(' ');
}

void	print_time(t_node *node)
{
	char	*file_time = ctime(&node->_stats_.st_mtimespec.tv_sec);

	if (!file_time)
		return ;
	file_time[16] = '\0';		/* don't print after 16th char (man ctime) */
	ft_add2buf(file_time + 4);	/* skip the day (first 4 chars) */
	ft_add_char2buf(' ');
}

void	print_color(t_node *node)
{
	if (S_ISLNK(node->_lstats_.st_mode))
		ft_add2buf(PURPLE);
	else if (DT_DIR != node->_dir_.d_type && node->_lstats_.st_mode & S_IXOTH)
		ft_add2buf(RED);
	else if (DT_DIR == node->_dir_.d_type)
		ft_add2buf(B_CYAN);
}


void	print_readlink(t_node *node)
{
	char	buf[FILENAME_MAX + 1] = {0};
	char	*filename;

	ft_asprintf(&filename, "%s/%s", node->path, node->_dir_.d_name);
	readlink(filename, buf, FILENAME_MAX);
	ft_memdel((void **)&filename);
	ft_add2buf(" -> ");
	ft_add2buf(buf);
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
		print_time(node);
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
	
	/* follow the link and print it */
	if (S_ISLNK(node->_lstats_.st_mode))
		print_readlink(node);

	/* add `/' when the option `-p' is turned on and if it's a directory */
	if (is_flag(OPT_P_MIN) && DT_DIR == node->_dir_.d_type)
		ft_add_char2buf('/');
	
	/* new line -- end of the entry */
	ft_add_char2buf('\n');
}

static void	__print_lst_recursively__(t_list *head, int is_last)
{
	t_list	*lst;
	t_node	*node;

	/* first print all the current list */
	ft_add2buf(((t_node *)head->content)->path);
	ft_add2buf(":\n");
	lst = head;
	while (lst)
	{
		node = (t_node *)lst->content;
		if (node)
			ft_print_entry((t_list *)head, node);
		lst = lst->next;
	}
	
	if (FALSE == is_last)
		ft_add_char2buf('\n');

	/* then print the recursive lists */
	lst = head;
	while (lst)
	{
		node = (t_node *)lst->content;
		if (node && is_flag(OPT_R_MAJ) && node->recursive_nodes)
		{
			__print_lst_recursively__(node->recursive_nodes, FALSE);
		}
		lst = lst->next;
	}
}

void	__print_entries__(t_list *head)
{
	t_list	*lst = head;

	while (lst)
	{
		// ft_add2buf(((t_node *)((t_list *)lst->content)->content)->path);
		// ft_add2buf(":\n");
		// ft_printf(B_RED "%s:\n" CLR_COLOR,
		// 	((t_node *)((t_list *)lst->content)->content)->_dir_.d_name);
		
		__print_lst_recursively__((t_list *)lst->content, (lst->next == NULL));
		lst = lst->next;
	}
}

int	main(int ac, char **av)
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
