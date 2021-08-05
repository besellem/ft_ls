/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: besellem <besellem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/15 22:22:52 by besellem          #+#    #+#             */
/*   Updated: 2021/08/05 17:55:18 by besellem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/*
** A call to the singleton returns the main structure. It's like a global.
** We can use it like this for example:
**
**   singleton()->buffer
*/
t_ls	*singleton(void)
{
	static t_ls	*s = NULL;

	if (!s)
	{
		s = (t_ls *)ft_calloc(1, sizeof(t_ls));
		if (!s)
			return (NULL);
		
		/*
		** If False, do not print colors, etc...
		** It will also, in this case, set errno != 0
		*/
		s->_isatty = isatty(STDOUT_FILENO);
		errno = 0;
	}
	return (s);
}
