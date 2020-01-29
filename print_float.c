/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_float.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gartanis <gartanis@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 15:03:05 by gartanis          #+#    #+#             */
/*   Updated: 2020/01/30 01:43:04 by gartanis         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static char	*rounding_float(char *str, int precis, int *len)
{
	int		k;
	char	*s_out;

	if (!(s_out = (char *)malloc(sizeof(char) * (ft_strlen(str) + precis))))
		return (0);
	while (str[++(*len)] != '.')
		s_out[*len] = str[*len];
	s_out[(*len)++] = (precis > 0) ? str[*len] : '\0';
	while (precis > 0 && str[*len])
	{
		if ((precis--) == 1)
		{
			s_out[*len] = (str[*len + 1] > '4') ? str[*len] + 1 : str[*len];
			k = *len;
			while ((s_out[k] > '9') ? s_out[k--] = '0' : 0)
				s_out[k] += 1;
		}
		else
			s_out[(*len)] = str[*len];
		*len += 1;
	}
	while (precis-- > 0)
		s_out[(*len)++] = '0';
	s_out[*len] = '\0';
	return (s_out);
}

static char	*printf_f(double args, int len, t_param *param)
{
	int				exp;
	int				*bin_dec;
	char			*str;
	unsigned char	str_mem[len + 1];

	if (!(str = (char *)malloc(sizeof(char) * 100)))
		return (0);
	ft_memcpy(str_mem, &args, len);
	while (--len >= 0)
		str = ft_strcat(str, get_bin(str_mem[len]));
	(*str == '1') ? param->t_flag.plus = '-' : 0;
	exp = get_exponent(str + 1, 10, 1023);
	bin_dec = get_mantisa(str + 12, &len, bin_dec);
	(exp >= 0) ? bin_dec = multiply_bigint(bin_dec, exp, &len) : 0;
	(exp < 0) ? bin_dec = divide_bigint(bin_dec, exp, &len) : 0;
	str = ft_dectostr(bin_dec, 0, len);
	return (str);
}

static char	*printf_lf(long double args, int len, t_param *param)
{
	int				exp;
	int				*bin_dec;
	char			*str;
	unsigned char	str_mem[len + 1];

	if (!(str = (char *)malloc(sizeof(char) * 100)))
		return (0);
	ft_memcpy(str_mem, &args, len);
	while (--len >= 0)
		str = ft_strcat(str, get_bin(str_mem[len]));
	(*str == '1') ? param->t_flag.plus = '-' : 0;
	exp = get_exponent(str + 1, 14, 16383);
	bin_dec = get_mantisa(str + 17, &len, bin_dec);
	(exp >= 0) ? bin_dec = multiply_bigint(bin_dec, exp, &len) : 0;
	(exp < 0) ? bin_dec = divide_bigint(bin_dec, exp, &len) : 0;
	str = ft_dectostr(bin_dec, 0, len);
	return (str);
}

static void	print_space_float(t_param *pm, int *len, char sign, char space)
{
	int i;

	space = ((space && !pm->width) || (space && pm->t_flag.zero)) ? ' ' : 0;
	i = pm->width - *len - (!sign ? 0 : 1) - ((space && !sign) ? 1 : 0);
	i = (i < 0) ? 0 : i;
	*len += i;
	if (!pm->t_flag.minus)
	{
		while (!pm->t_flag.zero && i-- > 0)
			ft_putchar(32);
		if (space || sign)
			ft_putchar(sign != 0 ? sign : space);
		while (pm->t_flag.zero && i-- > 0)
			ft_putchar(48);
	}
	else if (pm->t_flag.minus)
		while (i-- > 0)
			ft_putchar(32);
}

int			printf_float(va_list args, t_param *pm)
{
	int		len;
	char	*output;
	char	*str;

	(pm->t_flag.dot != DOT) ? pm->precision = 6 : 0;
	if (pm->modifier & UPP_L)
		str = printf_lf(va_arg(args, long double), 10, pm);
	else
		str = printf_f(va_arg(args, double), 8, pm);
	len = -1;
	str = rounding_float(str, pm->precision, &len);
	if (!pm->t_flag.minus)
		print_space_2(pm, &len, pm->t_flag.plus, pm->t_flag.space);
	(pm->t_flag.minus) ? ft_putchar(pm->t_flag.plus) : 0;
	ft_putstr(str);
	if (pm->t_flag.minus)
		print_space_2(pm, &len, pm->t_flag.plus, pm->t_flag.space);
	return (len);
}
