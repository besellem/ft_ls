/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_size.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/27 15:22:12 by besellem          #+#    #+#             */
/*   Updated: 2022/04/03 17:24:21 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

// #define UNIT_BYTE     1024.0					/* ~ 2^10 */
// #define UNIT_KILOBYTE 1048576.0					/* 2^20 */
// #define UNIT_MEGABYTE 1073741824.0				/* 2^30 */
// #define UNIT_GIGABYTE 1099511627776.0			/* 2^40 */
// #define UNIT_TERABYTE 1125899906842624.0		/* 2^50 */
// #define UNIT_PETABYTE 1152921504606846976.0		/* 2^60 */


#define UNIT_BYTE     1000.0					/* ~ 2^10 */
#define UNIT_KILOBYTE 1000000.0					/*   2^20 */
#define UNIT_MEGABYTE 1000000000.0				/*   2^30 */
#define UNIT_GIGABYTE 1000000000000.0			/*   2^40 */
#define UNIT_TERABYTE 1000000000000000.0		/*   2^50 */
#define UNIT_PETABYTE 1000000000000000000.0		/*   2^60 */

struct s_unit_lookup_table
{
	char	unit;
	double	size;
};

const struct s_unit_lookup_table	g_units[] = {
	{'B', UNIT_BYTE},
	{'K', UNIT_KILOBYTE},
	{'M', UNIT_MEGABYTE},
	{'G', UNIT_GIGABYTE},
	{'T', UNIT_TERABYTE},
	{'P', UNIT_PETABYTE},
	{0, 0}
};

void	print_size(t_node *node, t_pad *pads)
{
	char	*tmp = NULL;

	if (is_flag(OPT_H_MIN)) /* ISN'T GOOD */
	{
		for (int i = 0; g_units[i].unit != 0; ++i)
		{
			if (node->_stats_.st_size < g_units[i].size)
			{
				ft_asprintf(&tmp, "%*.1f%c",
					5,
					node->_stats_.st_size / (g_units[i].size / UNIT_BYTE),
					g_units[i].unit);
				break ;
			}
		}
	}
	else
		ft_asprintf(&tmp, "%*lld", pads->size, node->_stats_.st_size);
	if (!tmp)
		ft_free_exit();
	ft_buffadd(tmp);
	ft_memdel((void **)&tmp);
	ft_buffaddc(' ');
}
