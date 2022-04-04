/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/27 13:35:48 by besellem          #+#    #+#             */
/*   Updated: 2022/04/04 15:09:53 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int	ft_is_dir(char *path)
{
	DIR	*d = opendir(path);

	if (!d)
		return (FALSE);
	closedir(d);
	return (TRUE);
}
