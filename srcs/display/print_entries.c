/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_entries.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/27 13:59:55 by besellem          #+#    #+#             */
/*   Updated: 2021/06/27 15:27:55 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/* get max padding values of a list */
static void	__set_pads__(t_list *head, t_padding *pads)
{
	struct passwd	*password;
	struct group	*group;
	t_padding 		tmp_pads;
	t_list			*tmp;
	t_node			*node;

	ft_bzero(pads, sizeof(t_padding));
	ft_bzero(&tmp_pads, sizeof(t_padding));
	tmp = head;
	while (tmp)
	{
		node = (t_node *)tmp->content;
		if (node)
		{
			if (is_flag(OPT_S_MIN))
			{
				tmp_pads.blocks = ft_nblen(node->_stats_.st_blocks);
				if (tmp_pads.blocks > pads->blocks)
					pads->blocks = tmp_pads.blocks;
			}
			if (is_flag(OPT_L_MIN))
			{
				tmp_pads.nlink = ft_nblen(node->_stats_.st_nlink);
				if (tmp_pads.nlink > pads->nlink)
					pads->nlink = tmp_pads.nlink;
				
				password = getpwuid(node->_stats_.st_uid);
				tmp_pads.owner_name = ft_strlen(password->pw_name);
				if (tmp_pads.owner_name > pads->owner_name)
					pads->owner_name = tmp_pads.owner_name;

				group = getgrgid(node->_stats_.st_gid);
				tmp_pads.group_name = ft_strlen(group->gr_name);
				if (tmp_pads.group_name > pads->group_name)
					pads->group_name = tmp_pads.group_name;
				
				tmp_pads.size = ft_nblen(node->_stats_.st_size);
				if (tmp_pads.size > pads->size)
					pads->size = tmp_pads.size;
			}
		}
		tmp = tmp->next;
	}
}

static void	ft_print_entry(t_node *node, t_padding *pads)
{
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
	t_padding	pads;
	t_list		*lst;
	t_node		*node;

	/* set the different padding values */
	__set_pads__(head, &pads);

	/* first print all the current list */
	ft_add2buf(((t_node *)head->content)->path);
	ft_add2buf(":\n");
	lst = head;
	while (lst)
	{
		node = (t_node *)lst->content;
		if (node)
			ft_print_entry(node, &pads);
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

void	ft_print_entries(t_list *head)
{
	t_list	*lst = head;

	while (lst)
	{
		__print_lst_recursively__((t_list *)lst->content, (NULL == lst->next));
		lst = lst->next;
	}
}
