/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 21:49:11 by besellem          #+#    #+#             */
/*   Updated: 2022/03/28 23:16:31 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static const struct s_options	g_options[] = {
	{'A', OPT_A},
	{'a', OPT_A_MIN},
	{'B', OPT_B},
	{'b', OPT_B_MIN},
	{'C', OPT_C},
	{'c', OPT_C_MIN},
	{'d', OPT_D_MIN},
	{'e', OPT_E_MIN},
	{'F', OPT_F},
	{'f', OPT_F_MIN},
	{'G', OPT_G},
	{'g', OPT_G_MIN},
	{'H', OPT_H},
	{'h', OPT_H_MIN},
	{'i', OPT_I_MIN},
	{'k', OPT_K_MIN},
	{'L', OPT_L},
	{'l', OPT_L_MIN},
	{'m', OPT_M_MIN},
	{'n', OPT_N_MIN},
	{'O', OPT_O},
	{'o', OPT_O_MIN},
	{'P', OPT_P},
	{'p', OPT_P_MIN},
	{'q', OPT_Q_MIN},
	{'R', OPT_R},
	{'r', OPT_R_MIN},
	{'S', OPT_S},
	{'s', OPT_S_MIN},
	{'T', OPT_T},
	{'t', OPT_T_MIN},
	{'U', OPT_U},
	{'u', OPT_U_MIN},
	{'v', OPT_V_MIN},
	{'W', OPT_W},
	{'w', OPT_W_MIN},
	{'x', OPT_X_MIN},
	{'1', OPT_1},
	{0, 0}
};

// NOT QUITE FINISHED
void	resolve_options_conflicts(void)
{
	if (is_flag(OPT_F_MIN))
		add_flag(OPT_A_MIN);

	if (!is_flag(OPT_F) && !is_flag(OPT_L_MIN) && !is_flag(OPT_D))
		add_flag(OPT_H);

	if (is_flag(OPT_L))
		rm_flag(OPT_P);

	if (is_flag(OPT_N_MIN))
		add_flag(OPT_L_MIN);
}

void	illegal_opt(char opt)
{
	ft_printf(PROG_NAME ": illegal option -- %c\n"
			  "usage: " PROG_NAME " [-1ABCFGHLOPRSTUWabcdefghiklmnopqrstuwx] [file ...]\n",
			  opt);
}

int		get_args(const char *arg)
{
	int		got_valid_option;
	size_t	i;
	size_t	j;

	for (i = 0; arg[i]; ++i)
	{
		got_valid_option = FALSE;
		for (j = 0; g_options[j].opt; ++j)
		{
			if (g_options[j].opt == arg[i])
			{
				add_flag(g_options[j].flag);
				resolve_options_conflicts(); // ? NOT NECESSARY
				got_valid_option = TRUE;
				break ;
			}
		}
		if (FALSE == got_valid_option)
		{
			illegal_opt(arg[i]);
			return (FALSE);
		}
	}
	return (TRUE);
}

int		ft_parse_args(int ac, char **av, t_list **args)
{
	int				args_are_done = FALSE;
	struct stat		__stat;
	DIR				*dir;
	int				i;

	for (i = 1; i < ac; ++i)
	{
		if ('-' == av[i][0] && av[i][1] && FALSE == args_are_done)
		{
			if (FALSE == get_args(av[i] + 1))
				return (FALSE);
		}
		else
		{
			args_are_done = TRUE;
			errno = 0;
			dir = opendir(av[i]);
			int	tmp_errno = errno;
			if (!dir && NOT_FOUND == stat(av[i], &__stat))
				ft_dprintf(STDERR_FILENO, PROG_NAME ": %s: %s\n", av[i], strerror(tmp_errno));
			else
			{
				if (!ft_lst_push_back(args, (char *)av[i]))
					ft_free_exit();	
				if (dir)
					closedir(dir);
			}
		}
	}
	
	/* there may be conflicts to avoid in options (man ls) */
	resolve_options_conflicts();

	/* if there is no path after the options, do `ls' on the current path */
	if (0 == errno && ft_lstsize(*args) == 0)
	{
		if (!ft_lst_push_front(args, ".")) // `.' is the current directory
			ft_free_exit();	
	}
	// ft_lst_qsort(args, &cmp_node_by_desc);
	ft_sort_lst_nodes(args); /* PROBLEM HERE - CHECK : 'ls -lRrt incs srcs' */
	return (TRUE);
}
