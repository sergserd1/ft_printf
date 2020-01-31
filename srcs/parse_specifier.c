/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_specifier.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gartanis <gartanis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/26 16:16:44 by gartanis          #+#    #+#             */
/*   Updated: 2020/01/31 17:13:23 by gartanis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

void		print_space(int width, char flag)
{
	int i;

	i = 0;
	while (i++ < width)
		ft_putchar(flag);
}

int			find_percent(const char **pf)
{
	const char	*tmp;

	tmp = *pf;
	while (*tmp != '%')
		tmp += 1;
	if (*tmp == '%')
		return (1);
	return (PRINT_ERROR);
}

static int	check_specifier(char *dspecif, const char **pf, t_param *param)
{
	int			i;
	char		*pdsp;
	const char	*cp;

	i = 0;
	cp = *pf;
	while (*cp)
	{
		pdsp = dspecif;
		if (*cp == PERCENT)
			return (0);
		while (ft_strchr(FLAGS, *cp))
			cp += 1;
		while (ft_isdigit(*cp) || *cp == DOT)
			cp += 1;
		while (*cp == 'h' || *cp == 'l' || *cp == 'L')
			cp += 1;
		if (ft_strchr(pdsp, *cp))
			break ;
		else
			return (0);
	}
	param->specifier = *cp;
	return (1);
}

int			pars_specifier(const char **pf, t_param *param)
{
	int		ret;
	char	*dspecif;

	ret = PRINT_ERROR;
	dspecif = ft_strdup(SPECIFIER);
	if ((check_specifier(dspecif, pf, param)))
	{
		if (ft_strchr(dspecif, **pf))
		{
			*pf += 1;
			return (1);
		}
		ret = get_param(pf, param);
	}
	free(dspecif);
	return (ret);
}
