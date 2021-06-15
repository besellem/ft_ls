/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 21:36:34 by besellem          #+#    #+#             */
/*   Updated: 2021/06/15 23:14:42 by besellem         ###   ########.fr       */
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

# include "libft.h"

/*
** -- DEFINES --
*/
# define ERR_CODE 0
# define SUC_CODE 1

# define TRUE     1
# define FALSE    0

# define ERR() ft_printf(B_RED "%s:%d: " CLR_COLOR " Error\n", __FILE__, __LINE__);

/*
** -- DATA STRUCTURES --
*/
# define OPT_A_MAJ	(1ULL <<  0)
# define OPT_A_MIN	(1ULL <<  1)
# define OPT_B_MAJ	(1ULL <<  2)
# define OPT_B_MIN	(1ULL <<  3)
# define OPT_C_MAJ	(1ULL <<  4)
# define OPT_C_MIN	(1ULL <<  5)
# define OPT_D		(1ULL <<  6)
# define OPT_E		(1ULL <<  7)
# define OPT_F_MAJ	(1ULL <<  8)
# define OPT_F_MIN	(1ULL <<  9)
# define OPT_G_MAJ	(1ULL << 10)
# define OPT_G_MIN	(1ULL << 11)
# define OPT_H_MAJ	(1ULL << 12)
# define OPT_H_MIN	(1ULL << 13)
# define OPT_I		(1ULL << 14)
# define OPT_K		(1ULL << 15)
# define OPT_L_MAJ	(1ULL << 16)
# define OPT_L_MIN	(1ULL << 17)
# define OPT_M		(1ULL << 18)
# define OPT_N		(1ULL << 19)
# define OPT_O_MAJ	(1ULL << 20)
# define OPT_O_MIN	(1ULL << 21)
# define OPT_P_MAJ	(1ULL << 22)
# define OPT_P_MIN	(1ULL << 23)
# define OPT_Q		(1ULL << 24)
# define OPT_R_MAJ	(1ULL << 25)
# define OPT_R_MIN	(1ULL << 26)
# define OPT_S_MAJ	(1ULL << 27)
# define OPT_S_MIN	(1ULL << 28)
# define OPT_T_MAJ	(1ULL << 29)
# define OPT_T_MIN	(1ULL << 30)
# define OPT_U_MAJ	(1ULL << 31)
# define OPT_U_MIN	(1ULL << 32)
# define OPT_V		(1ULL << 33)
# define OPT_W_MAJ	(1ULL << 34)
# define OPT_W_MIN	(1ULL << 35)
# define OPT_X		(1ULL << 36)

struct	s_options{
	char		opt;
	uint64_t	flag;
};

/*
** opts:		flag containing all options parsed
*/
typedef	struct	s_ls
{
	uint64_t	opts;
	t_list		*lst;
}				t_ls;

/*
** -- PROTOTYPES --
** Utils
*/
void			ft_free_all(void);
t_ls			*singleton(void);

/* Parsing */
int				parse_args(int ac, const char **av);

#endif
