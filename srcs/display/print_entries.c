/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_entries.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/27 13:59:55 by besellem          #+#    #+#             */
/*   Updated: 2022/04/19 12:57:05 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/* get max padding values of a list */
static void	__set_pads__(const node_list_t *head, t_pad *pads)
{
	struct stat		_stats;
	struct passwd	*password;
	struct group	*group;
	t_pad 			tmp_pads = {0};
	t_node			*node;

	ft_bzero(pads, sizeof(*pads));
	for (node_list_t *tmp = (node_list_t *)head; tmp; tmp = tmp->next)
	{
		node = tmp->content;
		if (!node)
			continue ;


#ifdef __linux__
		/* if `-A' is set & the node's name starts is either `.' or `..', do not print that node */
		if (is_flag(OPT_A) && !is_flag(OPT_A_MIN) &&
			(STRISEQ(node->_dir_.d_name, "..") || STRISEQ(node->_dir_.d_name, ".")))
		{
			continue ;
		}

		/* if `-a' is not set & the node's name starts with a `.', do not print that node */
		if (!is_flag(OPT_A) && !is_flag(OPT_A_MIN) && !is_flag(OPT_D_MIN) &&
			0 == ft_strncmp(ft_basename(node->_dir_.d_name), ".", 1))
		{
			continue ;
		}
#else
		if (!is_flag(OPT_A_MIN) && 0 == ft_strncmp(ft_basename(node->_dir_.d_name), ".", 1))
		{
			continue ;
		}
#endif

		
		_stats = is_flag(OPT_L) ? node->_stats_ : node->_lstats_;

		if (is_flag(OPT_I_MIN))
		{
			tmp_pads.inode = ft_nblen(_stats.st_ino);
			pads->inode = ft_max(pads->inode, tmp_pads.inode);
		}
		if (is_flag(OPT_S_MIN))
		{
#ifdef __linux__
			tmp_pads.blocks = ft_nblen(_stats.st_blocks >> 1);
#else
			tmp_pads.blocks = ft_nblen(_stats.st_blocks);
#endif
			pads->blocks = ft_max(pads->blocks, tmp_pads.blocks);
		}
		if (is_flag(OPT_L_MIN | OPT_O_MIN))
		{
			tmp_pads.nlink = ft_nblen(_stats.st_nlink);
			pads->nlink = ft_max(pads->nlink, tmp_pads.nlink);
			
			password = getpwuid(_stats.st_uid);
			tmp_pads.owner_name = ft_strlen(password->pw_name);
			pads->owner_name = ft_max(pads->owner_name, tmp_pads.owner_name);

			group = getgrgid(_stats.st_gid);
			tmp_pads.group_name = ft_strlen(group->gr_name);
			pads->group_name = ft_max(pads->group_name, tmp_pads.group_name);
			
			if (is_flag(OPT_H_MIN) && _stats.st_size > 1024)
				tmp_pads.size = ft_nblen(_stats.st_size / 1024);
			else
				tmp_pads.size = ft_nblen(_stats.st_size);
			pads->size = ft_max(pads->size, tmp_pads.size);
		}

		/* total blocks to print */
#ifdef __linux__
		pads->total_blocks += _stats.st_blocks >> 1;
#else
		pads->total_blocks += _stats.st_blocks;
#endif
	}
}

static void	__print_entry__(const t_node *node, const t_pad *pads)
{
	/* if `-A' is set & the node's name starts is either `.' or `..', do not print that node */
	if (is_flag(OPT_A) && !is_flag(OPT_A_MIN) &&
		(STRISEQ(node->_dir_.d_name, "..") || STRISEQ(node->_dir_.d_name, ".")))
	{
		return ;
	}

	/* if `-a' is not set & the node's name starts with a `.', do not print that node */
	if (!is_flag(OPT_A) && !is_flag(OPT_A_MIN) && !is_flag(OPT_D_MIN) &&
		0 == ft_strncmp(ft_basename(node->_dir_.d_name), ".", 1))
	{
		return ;
	}

	if (node->_stats_.st_nlink == 0) // special case
		return ;

	if (is_flag(OPT_I_MIN))
		print_inode(node, pads);

	/* print nbr of blocks if `-s' is set */
	if (is_flag(OPT_S_MIN))
		print_blocks(node, pads);

	/* print long format if `-l' (ell) is set */
	if (is_flag(OPT_L_MIN | OPT_O_MIN))
	{
		print_permissions(node);
		print_nlinks(node, pads);
		print_owner(node, pads);
		if (!is_flag(OPT_O_MIN))
			print_group(node, pads);
		print_size(node, pads);
		print_time(node);
	}

	/*
	** print node's name
	*/
	/* option `-G' turns on the colors */
	if (is_flag(OPT_G) && singleton()->_isatty)
		print_color(node);
	
	/* print entry name */
	ft_buffadd(node->_dir_.d_name);

	if (is_flag(OPT_G) && singleton()->_isatty)
		ft_buffadd(CLR_COLOR);
	
	/* follow the link and print it */
	if (S_ISLNK(node->_lstats_.st_mode) && !is_flag(OPT_L))
		print_readlink(node);

	/* add `/' when the option `-p' is turned on and if it's a directory */
	if (is_flag(OPT_P_MIN) && (DT_DIR == node->_dir_.d_type || DT_LNK == node->_dir_.d_type))
		ft_buffaddc('/');
	
	/* new line -- end of the entry */
	ft_buffaddc('\n');

	if ((is_flag(OPT_L_MIN) || is_flag(OPT_O_MIN)) && is_flag(OPT_XATTR))
		print_xattrs(node);
}

static void	print_total_blocks(const t_pad *pads)
{
	char	*tmp = NULL;

	ft_asprintf(&tmp, "total %d\n", pads->total_blocks);
	if (!tmp)
		die();
	ft_buffadd(tmp);
	ft_memdel((void **)&tmp);
}

void	__print_entries_lst__(node_list_t *head, bool _print_dir_path, bool _printing_file_list)
{
	t_pad	pads;

	/* set the different padding values */
	__set_pads__(head, &pads);

	/* print path header */
	if (_print_dir_path)
	{
		ft_buffadd(head->content->path);
		ft_buffadd(":\n");
	}
	
	if (_printing_file_list && is_flag(OPT_L_MIN | OPT_O_MIN | OPT_S_MIN))
		print_total_blocks(&pads);
	
	for (node_list_t *lst = head; lst != NULL; lst = lst->next)
	{
		if (lst->content)
			__print_entry__(lst->content, &pads);
	}
}
