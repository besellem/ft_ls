/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 21:36:34 by besellem          #+#    #+#             */
/*   Updated: 2021/06/17 22:56:45 by besellem         ###   ########.fr       */
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

# define USAGE "usage: " PROG_NAME " [-ABCFGHLOPRSTUWabcdefghiklmnopqrstuwx] [file ...]\n"
# define ERR() ft_printf(B_RED "%s:%d: " CLR_COLOR " Error\n", __FILE__, __LINE__);

# define ERR_CODE 0
# define SUC_CODE 1

# define TRUE     1
# define FALSE    0

# define EMPTY    0

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
# define OPT_H_MIN	(1ULL << 13)
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

/*
** -- DATA STRUCTURES --
*/

/*
** Used temporarily for a lookup table in the parsing of the options
*/
struct	s_options{
	char		opt;
	uint64_t	flag;
};

/*
** One node contains the file / folder and its infos
*/
typedef	struct	s_node{
	struct dirent	_dir_;
	struct stat		_stats_;
	struct stat		_lstats_;
	t_list			*recursive_nodes;
}				t_node;

/*
** opts:	flag containing all parsed options.
** args:	list containing all parsed arguments (is temporary).
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
typedef	struct	s_ls
{
	uint64_t	opts;
	t_list		*args;
	t_list		*nodes;
}				t_ls;

/*
** -- PROTOTYPES --
** Utils
*/
t_ls			*singleton(void);
void			ft_free_node(void *);
void			ft_free_all(void);
void			add_flag(uint64_t);
void			rm_flag(uint64_t);
int				is_flag(uint64_t);

/* Options parsing */
int				parse_args(int, const char **);

#endif
