/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 21:36:34 by besellem          #+#    #+#             */
/*   Updated: 2022/04/08 17:10:43 by besellem         ###   ########.fr       */
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


/*
** defined for the lists templates
** must be defined before including libft.h
*/
#ifndef die
# define die() ft_free_exit()
#endif

# include "libft.h"

/*
** -- DEFINES --
*/

/* debug macro - to remove when finished */
// # define __DEBUG__

/* `@' option is not set in libft/incs/parse_args.h */
# define OPT_XATTR 0x8000000000000000


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


/* define the function to sort the list */
#define get_cmp_method()													   \
	(is_flag(OPT_R_MIN) ?                                                      \
		(is_flag(OPT_T_MIN) ? &cmp_node_by_asc_time : &cmp_node_by_desc) :     \
		(is_flag(OPT_T_MIN) ? &cmp_node_by_desc_time : &cmp_node_by_asc))


#define alloc_node() ({ \
	t_node *__tmp = ((t_node *)ft_calloc(1, sizeof(t_node))); \
	if (!__tmp) die(); \
	__tmp; \
})



/*
** -- DATA STRUCTURES --
*/

/* typedefs */
typedef	struct s_node	t_node;
typedef struct s_pad	t_pad;

/* list types creation */
CREATE_LST_TYPE(t_args, t_node *); // list of arguments (used on parsing only)
CREATE_LST_TYPE(node_list_t, t_node *); // list of nodes
CREATE_LST_TYPE(list_t, node_list_t *); // list of node lists


/* get the len of max values for padding */
struct s_pad
{
	int		nlink;
	int		size;
	int		blocks;
	int		inode;
	int		total_blocks;
	int		owner_name;
	int		group_name;
};


/*
** One node contains the file / folder and its infos
*/
struct s_node
{
	char			*path;
	node_list_t		*recursive_nodes;
	struct stat		_stats_;
	struct stat		_lstats_;
	struct dirent	_dir_;
};


/*
** opts:	flag containing all parsed options.
** args:	list containing all parsed arguments.
** nodes:	list containing all arguments and their data.
			`nodes->content' is a `node_list_t *'. The `->content' of this
			former list is a `t_node *' structure containing all data about that
			file/folder.
			If the `-R' option is set, there will be a recursive dive in the
			directories found, setting another list into that node, itself
			containing a node... until no more directories are found in that
			path.
			All nodes are sorted according to the sorting options parsed (`-t'
			or `-r' for example).
*/
typedef	struct	s_ls_data
{
	int			_isatty;
	uint64_t	opts;
	t_args		*args;
	list_t		*nodes;
}				t_ls_data;


/*
** -- PROTOTYPES --
*/

/* General Utils */
t_ls_data		*singleton(void);


/* Utils */
int				ft_is_dir(char *);


/* Memory & Error Management */
void			__free_node_lst__(node_list_t *);
void			ft_free_nodes(list_t **);
void			ft_free_all(void);


/* Sorting Utils */
int				cmp_node_by_asc_time(t_node *, t_node *);
int				cmp_node_by_desc_time(t_node *, t_node *);
int				cmp_node_by_asc(t_node *, t_node *);
int				cmp_node_by_desc(t_node *, t_node *);


/* Options parsing & flag utils */
void			add_flag(uint64_t);
void			rm_flag(uint64_t);
int				is_flag(uint64_t);

int				ft_parse_args(int, char **, t_args **);


/* Display */
void			print_inode(const t_node *, const t_pad *);
void			print_permissions(const t_node *);
void			print_blocks(const t_node *, const t_pad *);
void			print_nlinks(const t_node *, const t_pad *);
void			print_owner(const t_node *, const t_pad *);
void			print_group(const t_node *, const t_pad *);
void			print_size(const t_node *, const t_pad *);
void			print_time(const t_node *);
void			print_color(const t_node *);
void			print_readlink(const t_node *);
void			print_xattrs(const t_node *);

void			__print_lst_recursively__(node_list_t *, bool);
void			__print_nodes__(const list_t *);


#endif
