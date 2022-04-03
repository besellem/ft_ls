/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 21:36:34 by besellem          #+#    #+#             */
/*   Updated: 2022/04/03 18:38:49 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

/*
** -- INCLUDES --
*/
# include <unistd.h>
# include <stdint.h>
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
# define PROG_NAME "ft_ls"

# define TRUE      1
# define FALSE     0

# define FOUND     1
# define NOT_FOUND (-1)

# define EMPTY     0

# define NO_ERR    (-1)

/* buffer size - the actual buffer stores data before a syscall to `write' */
# define _LS_BUFSIZ_  BUFSIZ /* BUFSIZ == 1024. setting it to 4096 may be faster */

/* debug macro - to remove when finished */
# define __DEBUG__    TRUE

# if defined(__DEBUG__) && (TRUE == __DEBUG__)
#  define ERR() ft_printf(B_RED "%s:%d: " CLR_COLOR " Error\n", __FILE__, __LINE__);
#  define LOG   ERR()
# else
#  define ERR()
#  define LOG
# endif

#if defined(__DEBUG__)
# define LST_DEBUG(lst)															\
	do {																		\
		t_list	*tmp = lst;														\
																				\
		printf(B_BLUE"%s:%d: "CLR_COLOR"lst_size: ["B_GREEN"%d"CLR_COLOR"]\n",	\
			__FILE__, __LINE__, ft_lstsize(lst));								\
		while (tmp) {															\
			printf("["B_RED"%p"CLR_COLOR"] ["B_RED"%p"CLR_COLOR"]\n",			\
				tmp, tmp->next);												\
			tmp = tmp->next;													\
		}																		\
	} while (0);
# else
# define LST_DEBUG(lst)	(void lst);
#endif /* defined(__DEBUG__) */

// # define merror() ft_printf("%s:%d: malloc error\n", __FILE__, __LINE__)

# define ft_free_exit()                                                        \
	do {                                                                       \
		ft_printf("%s:%d: %s\n", __FILE__, __LINE__, strerror(errno));         \
		ft_free_all();                                                         \
		exit(EXIT_FAILURE);                                                    \
	} while (0);


/*
** On M1 macs, the variadic functions are implemented differently.
** Thus, my printf() are buggy on this platform.
** To avoid this, I replace the use of ft_*printf by the real ones.
*/
#ifdef __arm64__
# define ft_printf(__fmt, ...)           printf((__fmt), ##__VA_ARGS__)
# define ft_dprintf(__fd, __fmt, ...)    dprintf((__fd), (__fmt), ##__VA_ARGS__)
# define ft_asprintf(__ptr, __fmt, ...)  asprintf((__ptr), (__fmt), ##__VA_ARGS__)
#endif

/*
** -- DATA STRUCTURES --
*/
enum e_error_msg
{
	ERR_MSG_MALLOC
};

/* Used temporarily for a lookup table in the option's parsing */
struct	s_options
{
	char		opt;
	uint64_t	flag;
};

/* get the len of max values for padding */
typedef struct s_pad
{
	int		nlink;
	int		size;
	int		blocks;
	int		total_blocks;
	int		owner_name;
	int		group_name;
}	t_pad;

/*
** One node contains the file / folder and its infos
*/
typedef	struct	s_node
{
	char			*path;
	t_list			*recursive_nodes;
	// t_pad		pad;
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
** buf_idx:	index into the current bufferized data
** buffer:	buffer containing the data to display. Just way faster than a lot of
			calls to `write'
*/
typedef	struct	s_ls_data
{
	int			_isatty;
	uint64_t	opts;
	t_list		*args;
	t_list		*nodes;
	size_t		buf_idx;
	char		buffer[_LS_BUFSIZ_];
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
void			print_blocks(t_node *, t_pad *);
void			print_permissions(t_node *);
void			print_nlinks(t_node *, t_pad *);
void			print_owner(t_node *, t_pad *);
void			print_group(t_node *, t_pad *);
void			print_size(t_node *, t_pad *);
void			print_time(t_node *);
void			print_color(t_node *);
void			print_readlink(t_node *);

void			ft_print_entries(t_list *);

void	ft_lstprint(t_list *lst); // debug

#endif
