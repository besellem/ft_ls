/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 21:36:34 by besellem          #+#    #+#             */
/*   Updated: 2021/11/05 15:52:34 by besellem         ###   ########.fr       */
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

# define USAGE "usage: " PROG_NAME " [-1ABCFGHLOPRSTUWabcdefghiklmnopqrstuwx] [file ...]\n"

# define SUC_CODE  1
# define ERR_CODE  0

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
# else
#  define ERR() (NULL);
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

# define ft_free_exit(val, error)                                              \
	do {                                                                       \
		ft_printf("%s:%d: %s\n", __FILE__, __LINE__, strerror(errno));         \
		ft_free_all();                                                         \
		exit(val);                                                             \
	} while (0);


# define OPT_A_MAJ	(1ULL <<  0)
# define OPT_A_MIN	(1ULL <<  1)	/* MANDATORY */
# define OPT_B_MAJ	(1ULL <<  2)
# define OPT_B_MIN	(1ULL <<  3)
# define OPT_C_MAJ	(1ULL <<  4)
# define OPT_C_MIN	(1ULL <<  5)
# define OPT_D		(1ULL <<  6)	/* Bonus */
# define OPT_E		(1ULL <<  7)
# define OPT_F_MAJ	(1ULL <<  8)
# define OPT_F_MIN	(1ULL <<  9)	/* Bonus */
# define OPT_G_MAJ	(1ULL << 10)
# define OPT_G_MIN	(1ULL << 11)	/* Bonus */
# define OPT_H_MAJ	(1ULL << 12)
# define OPT_H_MIN	(1ULL << 13)	/* DOES NOT WORK */
# define OPT_I		(1ULL << 14)
# define OPT_K		(1ULL << 15)
# define OPT_L_MAJ	(1ULL << 16)
# define OPT_L_MIN	(1ULL << 17)	/* MANDATORY */
# define OPT_M		(1ULL << 18)
# define OPT_N		(1ULL << 19)
# define OPT_O_MAJ	(1ULL << 20)
# define OPT_O_MIN	(1ULL << 21)
# define OPT_P_MAJ	(1ULL << 22)
# define OPT_P_MIN	(1ULL << 23)
# define OPT_Q		(1ULL << 24)
# define OPT_R_MAJ	(1ULL << 25)	/* MANDATORY */
# define OPT_R_MIN	(1ULL << 26)	/* MANDATORY */
# define OPT_S_MAJ	(1ULL << 27)
# define OPT_S_MIN	(1ULL << 28)
# define OPT_T_MAJ	(1ULL << 29)
# define OPT_T_MIN	(1ULL << 30)	/* MANDATORY */
# define OPT_U_MAJ	(1ULL << 31)
# define OPT_U_MIN	(1ULL << 32)	/* Bonus */
# define OPT_V		(1ULL << 33)
# define OPT_W_MAJ	(1ULL << 34)
# define OPT_W_MIN	(1ULL << 35)
# define OPT_X		(1ULL << 36)
# define OPT_ONE	(1ULL << 37)

/*
** -- DATA STRUCTURES --
*/

/*
** Used temporarily for a lookup table in the option's parsing
*/

enum e_error_msg
{
	ERR_MSG_MALLOC
};

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
	// t_pad		pad;
	struct dirent	_dir_;
	struct stat		_stats_;
	struct stat		_lstats_;
	t_list			*recursive_nodes;
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
typedef	struct	s_ls
{
	int			_isatty;
	uint64_t	opts;
	t_list		*args;
	t_list		*nodes;
	size_t		buf_idx;
	char		buffer[_LS_BUFSIZ_];
}				t_ls;

/*
** -- PROTOTYPES --
** General Utils
*/
t_ls			*singleton(void);

/* Utils */
int				ft_is_dir(char *);

/* Memory & Error Management */
void			ft_free_all(void);
// void			ft_free_exit(int val, enum e_error_msg __error);

/* Sorting Utils */
int				cmp_node_by_asc_time(t_node *, t_node *);
int				cmp_node_by_desc_time(t_node *, t_node *);
int				cmp_node_by_asc(t_node *, t_node *);
int				cmp_node_by_desc(t_node *, t_node *);
int				cmp_content_asc(void *, void *);
int				cmp_content_desc(void *, void *);

void			ft_lst_sort(t_list **, int (*)());  /* sort a list (slow) */
void			ft_lst_qsort(t_list **, int (*)()); /* quicksort a list */
void			ft_sort_lst_nodes(t_list **);

/* Options parsing & flag utils */
void			add_flag(uint64_t);
void			rm_flag(uint64_t);
int				is_flag(uint64_t);
int				parse_args(int, char **, t_list **);


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
