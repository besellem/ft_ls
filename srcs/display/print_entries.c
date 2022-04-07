/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_entries.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/27 13:59:55 by besellem          #+#    #+#             */
/*   Updated: 2022/04/07 22:45:59 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/* get max padding values of a list */
static void	__set_pads__(t_list *head, t_pad *pads)
{
	struct stat		_stats;
	struct passwd	*password;
	struct group	*group;
	t_pad 			tmp_pads = {0};
	t_node			*node;

	ft_bzero(pads, sizeof(t_pad));
	for (t_list *tmp = head; tmp; tmp = tmp->next)
	{
		node = (t_node *)tmp->content;
		if (!node)
			continue ;

		_stats = is_flag(OPT_L) ? node->_stats_ : node->_lstats_;

		if (is_flag(OPT_S_MIN))
		{
			tmp_pads.blocks = ft_nblen(_stats.st_blocks);
			if (tmp_pads.blocks > pads->blocks)
				pads->blocks = tmp_pads.blocks;
		}
		if (is_flag(OPT_L_MIN))
		{
			tmp_pads.nlink = ft_nblen(_stats.st_nlink);
			if (tmp_pads.nlink > pads->nlink)
				pads->nlink = tmp_pads.nlink;
			
			password = getpwuid(_stats.st_uid);
			tmp_pads.owner_name = ft_strlen(password->pw_name);
			if (tmp_pads.owner_name > pads->owner_name)
				pads->owner_name = tmp_pads.owner_name;

			group = getgrgid(_stats.st_gid);
			tmp_pads.group_name = ft_strlen(group->gr_name);
			if (tmp_pads.group_name > pads->group_name)
				pads->group_name = tmp_pads.group_name;
			
			tmp_pads.size = ft_nblen(_stats.st_size);
			if (tmp_pads.size > pads->size)
				pads->size = tmp_pads.size;
		}
		
		/* total blocks to print */
		pads->total_blocks += _stats.st_blocks; // TODO: diff btw `-l' & `-la'
	}
}

static void	ft_print_entry(const t_node *node, const t_pad *pads)
{

	/* if `-A' is set & the node's name starts is either `.' or `..', do not print that node */
	if (is_flag(OPT_A) && !is_flag(OPT_A_MIN) &&
		(0 == ft_strcmp(node->_dir_.d_name, "..") ||
		 0 == ft_strcmp(node->_dir_.d_name, ".")))
	{
		return ;
	}

	/* if `-a' is not set & the node's name starts with a `.', do not print that node */
	if (!is_flag(OPT_A) && !is_flag(OPT_A_MIN) &&
		0 == ft_strncmp(ft_basename(node->_dir_.d_name), ".", 1))
	{
		return ;
	}

	/* print nbr of blocks if `-s' is set */
	if (is_flag(OPT_S_MIN))
		print_blocks(node, pads);

	/* print long format if `-l' (ell) is set */
	if (is_flag(OPT_L_MIN))
	{
		print_permissions(node);
		print_nlinks(node, pads);
		print_owner(node, pads);
		print_group(node, pads);
		print_size(node, pads);
		print_time(node);
	}

	/*
	** print node's name
	*/
	/* option `-G' turns on the colors */
	if (is_flag(OPT_G) && TRUE == singleton()->_isatty)
		print_color(node);
	
	/* print entry name */
	ft_buffadd(node->_dir_.d_name);

	if (is_flag(OPT_G) && TRUE == singleton()->_isatty)
		ft_buffadd(CLR_COLOR);
	
	/* follow the link and print it */
	if (S_ISLNK(node->_lstats_.st_mode) && !is_flag(OPT_L))
		print_readlink(node);

	/* add `/' when the option `-p' is turned on and if it's a directory */
	if (is_flag(OPT_P_MIN) && DT_DIR == node->_dir_.d_type)
		ft_buffaddc('/');
	
	/* new line -- end of the entry */
	ft_buffaddc('\n');

	if (is_flag(OPT_L_MIN) && is_flag(OPT_XATTR))
		print_xattrs(node);
}

static void	print_total_blocks(t_pad *pads)
{
	char	*tmp = NULL;

	ft_asprintf(&tmp, "total %d\n", pads->total_blocks);
	if (!tmp)
		ft_free_exit();
	ft_buffadd(tmp);
	ft_memdel((void **)&tmp);
}

// static
void	__print_lst_recursively__(t_list *head, bool _print_dir_path)
{
	t_list	*lst;
	t_node	*node;
	t_pad	pads;

	/* set the different padding values */
	__set_pads__(head, &pads);

	/* first print all the current list */
	if (_print_dir_path)
	{
		ft_buffadd(((t_node *)head->content)->path);
		ft_buffadd(":\n");
	}
	
	if (is_flag(OPT_L_MIN))
	{
		print_total_blocks(&pads);
	}
	
	for (lst = head; lst != NULL; lst = lst->next)
	{
		node = (t_node *)lst->content;
		
		if (node)
			ft_print_entry(node, &pads);
	}

	/* then print the recursive lists */
	for (lst = head; lst != NULL; lst = lst->next)
	{
		node = (t_node *)lst->content;
		
		if (node && node->recursive_nodes && is_flag(OPT_R))
		{
			ft_buffaddc('\n');
			__print_lst_recursively__(node->recursive_nodes, _print_dir_path);
		}
	}
}

// void	__print_lst__(t_list *head, bool _print_dir_path)
// {
// 	t_list	*lst;
// 	t_node	*node;
// 	t_pad	pads;

// 	/* set the different padding values */
// 	__set_pads__(head, &pads);

// 	/* first print all the current list */
// 	if (_print_dir_path)
// 	{
// 		ft_buffadd(((t_node *)head->content)->path);
// 		ft_buffadd(":\n");
// 	}
	
// 	if (is_flag(OPT_L_MIN))
// 	{
// 		print_total_blocks(&pads);
// 	}
	
// 	for (lst = head; lst != NULL; lst = lst->next)
// 	{
// 		node = (t_node *)lst->content;
		
// 		if (node)
// 			ft_print_entry(node, &pads);
// 	}
// }

void	__print_node(void *data)
{
	t_node	*node = (t_node *)data;

	ft_printf("[%11p] [%d] [%s]\n",
		node, ft_lstsize(node->recursive_nodes), node->_dir_.d_name);
}


/*
** Check if there is some recursive nodes to print the path header
** Try `ls -1 srcs' vs `ls -1 srcs incs' :
** In the first, the path is not printed because there's no other path to print
** atferward.
*/
static bool	print_dir_path_check(void)
{
	t_list	*head = singleton()->nodes;
	bool	print_dir_path = (ft_lstsize(head) > 1);

	for (t_list *lst = head; lst; lst = lst->next)
	{
		if (print_dir_path)
			break ;
		
		for (t_list *_member = (t_list *)lst->content; _member; _member = _member->next)
		{
			t_node	*node = (t_node *)_member->content;

			if (node && node->recursive_nodes)
			{
				print_dir_path = true;
				break ;
			}
		}
	}
	return print_dir_path;
}

void	__print_nodes__(t_list *head)
{
	bool	print_dir_path = print_dir_path_check();

	for (t_list *lst = head; lst; lst = lst->next)
	{
		__print_lst_recursively__((t_list *)lst->content, print_dir_path);
		if (lst->next)
			ft_buffaddc('\n');
	}
}
