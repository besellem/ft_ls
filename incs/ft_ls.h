/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 21:36:34 by besellem          #+#    #+#             */
/*   Updated: 2022/04/04 17:34:39 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

/*
** -- INCLUDES --
*/
# include <unistd.h>
# include <stdint.h>
# include <stdbool.h>
# include <stdlib.h>
# include <dirent.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <pwd.h>
# include <uuid/uuid.h>
# include <grp.h>
# include <uuid/uuid.h>
# include <sys/xattr.h>
# include <time.h>
# include <stdio.h>
# include <errno.h>

# include "libft.h"

/*
** -- DEFINES --
*/

/* debug macro - to remove when finished */
# define __DEBUG__


/*
** On M1 macs, variadic functions are implemented differently.
** Because of that, my ft_*printf functions may be buggy on this platform.
** To avoid this, we can replace ft_*printf calls by the real ones
*/
// #ifdef __arm64__
// # define ft_printf(__fmt, ...)           printf((__fmt), ##__VA_ARGS__)
// # define ft_dprintf(__fd, __fmt, ...)    dprintf((__fd), (__fmt), ##__VA_ARGS__)
// # define ft_asprintf(__ptr, __fmt, ...)  asprintf((__ptr), (__fmt), ##__VA_ARGS__)
// #endif


# ifdef __DEBUG__
#  define LOG ft_printf(B_RED "%s:%d: " CLR_COLOR " Error\n", __FILE__, __LINE__);
#  define ft_free_exit()                                                       \
	do {                                                                       \
		ft_printf("%s:%d: %s\n", __FILE__, __LINE__, strerror(errno));         \
		ft_free_all();                                                         \
		exit(EXIT_FAILURE);                                                    \
	} while (0);
# else
#  define LOG
#  define ft_free_exit()                                                       \
	do {                                                                       \
		ft_free_all();                                                         \
		exit(EXIT_FAILURE);                                                    \
	} while (0);
# endif


/*
** -- DATA STRUCTURES --
*/

/* get the len of max values for padding */
typedef struct	s_pad
{
	int		nlink;
	int		size;
	int		blocks;
	int		total_blocks;
	int		owner_name;
	int		group_name;
}				t_pad;

/*
** One node contains the file / folder and its infos
*/
typedef	struct	s_node
{
	char			*path;
	t_list			*recursive_nodes;
	struct stat		_stats_;
	struct stat		_lstats_;
	struct dirent	_dir_;
}				t_node;

/*
** opts:	flag containing all parsed options.
** args:	list containing all parsed arguments.
** nodes:	list containing all arguments and their data.
			`nodes->content' is a `t_list *'. The `lst->content' of this former
			list is a `t_node *' structure containing all data about that
			file / folder.
			If the `-R' option is set, there will be a recursive dive in the
			directories found, setting another list into that node, itself
			containing a node... until no more directories are found in that
			path.
			All nodes are sorted according to the sorting options set (`-t' or
			`-r' for example).
*/
typedef	struct	s_ls_data
{
	int			_isatty;
	uint64_t	opts;
	t_list		*args;
	t_list		*nodes;
}				t_ls_data;

/*
** -- PROTOTYPES --
** General Utils
*/
t_ls_data		*singleton(void);

/* Utils */
int				ft_is_dir(char *);

/* Memory & Error Management */
void			ft_free_all(void);


/* Sorting Utils */
int				cmp_node_by_asc_time(t_node *, t_node *);
int				cmp_node_by_desc_time(t_node *, t_node *);
int				cmp_node_by_asc(t_node *, t_node *);
int				cmp_node_by_desc(t_node *, t_node *);
int				cmp_content_asc(void *, void *);
int				cmp_content_desc(void *, void *);

void			ft_sort_lst_nodes(t_list **);


/* Options parsing & flag utils */
void			add_flag(uint64_t);
void			rm_flag(uint64_t);
int				is_flag(uint64_t);

int				ft_parse_args(int, char **, t_list **);


/* Display */
void			print_blocks(const t_node *, const t_pad *);
void			print_permissions(const t_node *);
void			print_nlinks(const t_node *, const t_pad *);
void			print_owner(const t_node *, const t_pad *);
void			print_group(const t_node *, const t_pad *);
void			print_size(const t_node *, const t_pad *);
void			print_time(const t_node *);
void			print_color(const t_node *);
void			print_readlink(const t_node *);

void			__print_lst_recursively__(t_list *, bool);
void			__print_nodes__(t_list *);

#endif
