/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 21:49:11 by besellem          #+#    #+#             */
/*   Updated: 2021/06/15 23:13:05 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

#define OPTIONS_NB 36

const struct s_options	g_options[] = {
	{'A', OPT_A_MAJ},
	{'a', OPT_A_MIN},
	{'B', OPT_B_MAJ},
	{'b', OPT_B_MIN},
	{'C', OPT_C_MAJ},
	{'c', OPT_C_MIN},
	{'d', OPT_D},
	{'e', OPT_E},
	{'F', OPT_F_MAJ},
	{'f', OPT_F_MIN},
	{'G', OPT_G_MAJ},
	{'g', OPT_G_MIN},
	{'H', OPT_H_MAJ},
	{'h', OPT_H_MIN},
	{'i', OPT_I},
	{'k', OPT_K},
	{'L', OPT_L_MAJ},
	{'l', OPT_L_MIN},
	{'m', OPT_M},
	{'n', OPT_N},
	{'O', OPT_O_MAJ},
	{'o', OPT_O_MIN},
	{'P', OPT_P_MAJ},
	{'p', OPT_P_MIN},
	{'q', OPT_Q},
	{'R', OPT_R_MAJ},
	{'r', OPT_R_MIN},
	{'S', OPT_S_MAJ},
	{'s', OPT_S_MIN},
	{'T', OPT_T_MAJ},
	{'t', OPT_T_MIN},
	{'U', OPT_U_MAJ},
	{'u', OPT_U_MIN},
	{'v', OPT_V},
	{'W', OPT_W_MAJ},
	{'w', OPT_W_MIN},
	{'x', OPT_X},
	{'\0', 0}
};

void	get_args(const char *arg)
{
	size_t	i = 0;
	size_t	j;

	while (arg[i])
	{
		j = 0;
		while (g_options[j].opt)
		{
			if (g_options[j].opt == arg[i])
			{
				singleton()->opts |= g_options[j].flag;
				break ;
			}
			++j;
		}
		++i;
	}
}

// NOT QUITE FINISHED
void	resolve_options_conflicts(void)
{
	const uint64_t	flag = singleton()->opts;

	if (flag & OPT_F_MIN)
	{
		singleton()->opts |= OPT_A_MIN;
	}
	if (0 == (flag & OPT_F_MAJ) &&
		0 == (flag & OPT_L_MIN) &&
		0 == (flag & OPT_D))
	{
		singleton()->opts |= OPT_H_MAJ;
	}
	if (flag & OPT_L_MAJ)
	{
		singleton()->opts &= ~OPT_P_MAJ;
	}
	if (flag & OPT_N)
	{
		singleton()->opts |= OPT_L_MIN;
	}
}

int	parse_args(int ac, const char **av)
{
	t_list	*new;
	int		args_are_done = FALSE;

	for (int i = 1; i < ac; ++i)
	{
		if ('-' == av[i][0] && FALSE == args_are_done)
		{
			get_args(av[i] + 1);
		}
		else
		{
			args_are_done = TRUE;
			new = ft_lstnew((char *)av[i]);
			if (!new)
				return (ERR_CODE);
			ft_lstadd_back(&singleton()->lst, new);
		}
	}
	// resolve_options_conflicts();
	return (SUC_CODE);
}
