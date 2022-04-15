/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 21:36:34 by besellem          #+#    #+#             */
/*   Updated: 2022/04/15 18:45:27 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

/*
** -- INCLUDES --
*/
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <uuid/uuid.h>
#include <grp.h>
#include <uuid/uuid.h>
#include <sys/xattr.h>
#include <sys/acl.h>
#include <time.h>
#include <stdio.h>
#include <errno.h>


/*
** defined for the lists templates
** must be defined before including libft.h
*/
#ifndef die
# define die() ft_free_exit()
#endif

#include "libft.h"

/*
** -- DEFINES --
*/

/* debug macro - to remove when finished */
// # define __DEBUG__

#define HANDLED_FLAGS  "-1ABCFGHLOPRSTUWabcdefghiklmnopqrstuwx"

/* `@' option is not set in libft/incs/parse_args.h */
#define OPT_XATTR       0x8000000000000000


#define LOG ft_printf(B_RED "%s:%d: " CLR_COLOR " Error\n", __FILE__, __LINE__);

#ifdef __DEBUG__
# define ft_free_exit()                                                        \
	do {                                                                       \
		ft_printf("%s:%d: %s\n", __FILE__, __LINE__, strerror(errno));         \
		exit(EXIT_FAILURE);                                                    \
	} while (0);
#else
# define ft_free_exit() exit(EXIT_FAILURE)
#endif


#if defined(__APPLE__) && defined(__MACH__) /* macOS */
# define _atime_spec(__st) ((__st).st_atimespec)
# define _mtime_spec(__st) ((__st).st_mtimespec)
# define _ctime_spec(__st) ((__st).st_ctimespec)
#else                                       /* Linux */
# if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
#  define LINUX_OLD_KERNEL_VERSION
#  error "Kernel version too old: require 2.6.0 or newer"
# else
#  define _atime_spec(__st) ((__st).st_atime)
#  define _mtime_spec(__st) ((__st).st_mtime)
#  define _ctime_spec(__st) ((__st).st_ctime)
# endif
#endif /* defined(__APPLE__) && defined(__MACH__) */


/* define the function to sort the list */
#define get_cmp_method()                                                       \
	(is_flag(OPT_R_MIN) ?                                                      \
		(is_flag(OPT_U_MIN) ?                                                  \
			&cmp_node_by_asc_last_access_time :                                \
			is_flag(OPT_T_MIN) ? &cmp_node_by_asc_time : &cmp_node_by_desc) :  \
		(is_flag(OPT_U_MIN) ?                                                  \
			&cmp_node_by_desc_last_access_time :                               \
			is_flag(OPT_T_MIN) ? &cmp_node_by_desc_time : &cmp_node_by_asc))


#define alloc_node() ({ \
	t_node *__ptr = ((t_node *)ft_calloc(1, sizeof(t_node)));                  \
	if (!__ptr)                                                                \
		die();                                                                 \
	__ptr;                                                                     \
})


#define ft_max(a, b) ({                                                        \
	typeof(a) _a = (a);                                                        \
	typeof(b) _b = (b);                                                        \
    _a > _b ? _a : _b;                                                         \
})

#define ft_min(a, b) ({                                                        \
	typeof(a) _a = (a);                                                        \
	typeof(b) _b = (b);                                                        \
	_a < _b ? _a : _b;                                                         \
})

#define STRISEQ(__s1, __s2)  (0 == ft_strcmp((__s1), (__s2)))


/*
** -- DATA STRUCTURES --
*/

/* typedefs */
typedef	struct s_node	t_node;
typedef struct s_pad	t_pad;

/* list types creation */
CREATE_LST_TYPE(t_args, t_node *) // list of arguments (used on parsing only)

CREATE_LST_TYPE(node_list_t, t_node *) // list of nodes


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
	char			*constructed_path;
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
typedef struct s_ls_data
{
	int			_isatty;
	uint64_t	opts;
	t_args		*args;
}				t_ls_data;


/*
** -- PROTOTYPES --
*/

/* General Utils */
t_ls_data		*singleton(void) __constructor;


/* Utils */
int				ft_is_dir(char *);


/* Memory & Error Management */
void			rm_arg(t_args **, t_args *);
void			free_node_lst(node_list_t *);
void			ft_free_all(void) __destructor;


/* Sorting Utils */
int				cmp_node_by_asc_time(t_node *, t_node *);
int				cmp_node_by_desc_time(t_node *, t_node *);
int				cmp_node_by_asc_last_access_time(t_node *, t_node *);  // unused
int				cmp_node_by_desc_last_access_time(t_node *, t_node *); // unused
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

void			__print_entries_lst__(node_list_t *, bool, bool);

#endif
