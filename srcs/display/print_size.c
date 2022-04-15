/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_size.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/27 15:22:12 by besellem          #+#    #+#             */
/*   Updated: 2022/04/15 18:42:18 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

#define UNIT_BYTE     1024.0                 /* 2^10 */
#define UNIT_KILOBYTE 1048576.0              /* 2^20 */
#define UNIT_MEGABYTE 1073741824.0           /* 2^30 */
#define UNIT_GIGABYTE 1099511627776.0        /* 2^40 */
#define UNIT_TERABYTE 1125899906842624.0     /* 2^50 */
#define UNIT_PETABYTE 1152921504606846976.0  /* 2^60 */

const struct s_unit_lookup_table
{
	char	unit;
	double	size;
}	g_units[] = {
	{'B', UNIT_BYTE},
	{'K', UNIT_KILOBYTE},
	{'M', UNIT_MEGABYTE},
	{'G', UNIT_GIGABYTE},
	{'T', UNIT_TERABYTE},
	{'P', UNIT_PETABYTE},
	{0, 0.}
};

void	print_size(const t_node *node, const t_pad *pads)
{
	const off_t	_size = is_flag(OPT_L) ? node->_stats_.st_size : node->_lstats_.st_size;
	char		*tmp = NULL;

	if (is_flag(OPT_H_MIN)) // TODO: debug this
	{
		for (int i = 0; g_units[i].unit != 0; ++i)
		{
			if (_size < g_units[i].size)
			{
				const int	pad_size = ft_min(pads->size, 4) + 1; // TODO: debug this

				if ('B' == g_units[i].unit)
					ft_asprintf(&tmp, "%*lld%c ", pad_size, _size, g_units[i].unit);
				else
					ft_asprintf(&tmp, "%*.1f%c ", pad_size, _size / (g_units[i].size / UNIT_BYTE), g_units[i].unit);
				break ;
			}
		}
	}
	else
	{
		ft_asprintf(&tmp, "%*lld ", pads->size, _size);
	}
	if (!tmp)
		ft_free_exit();
	ft_buffadd(tmp);
	ft_memdel((void **)&tmp);
}
