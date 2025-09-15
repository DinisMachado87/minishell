/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normalizer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dimachad <dimachad@student.42berlin.d>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:10:46 by dimachad          #+#    #+#             */
/*   Updated: 2025/08/18 23:01:59 by dimachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stdlib.h>

typedef struct	s_i_count {
	int	ltr;
	int	chr;
}	t_i_count;

typedef struct	s_i_cpy {
	int	src;
	int	chr;
	int	dest;
}	t_i_cpy;

static inline int	needs_space_before(const char *str, int i)
{
	return (i > 0 && str[i - 1] != ' ' && str[i - 1] != str[i]);
}

static inline int	needs_space_after(const char *str, int i)
{
	return (str[i + 1] && str[i + 1] != ' ' && str[i + 1] != str[i]);
}

int	count_spaced_chars(char *str)
{
	const char *spec_chr = "<>&|";
	t_i_count	i;
	int	extra_spaces;

	i.ltr = 0;
	i.chr = 0;
	extra_spaces = 0;
	while (str[i.ltr])
	{
		while (spec_chr[i.chr] && str[i.ltr] != spec_chr[i.chr])
			i.chr++;
		if (spec_chr[i.chr] && needs_space_before(str, i.ltr))
			extra_spaces++;
		if (spec_chr[i.chr] && needs_space_after(str, i.ltr))
			extra_spaces++;
		i.ltr++;
		i.chr = 0;
	}
	return (i.ltr + extra_spaces);
}

void	copy_str_with_extra_spaces(char *src, char *dest)
{
	const char *ref_chr = "<>&|";
	t_i_cpy i;

	i.src = 0;
	i.chr = 0;
	i.dest = 0;
	while (src[i.src])
	{
		while (ref_chr[i.chr] && src[i.src] != ref_chr[i.chr])
			i.chr++;
		if (ref_chr[i.chr] && needs_space_before(src, i.src))
			dest[i.dest++] = ' ';
		dest[i.dest++] = src[i.src];
		if (ref_chr[i.chr] && needs_space_after(src, i.src))
			dest[i.dest++] = ' ';
		i.src++;
		i.chr = 0;
	}
	dest[i.dest] = '\0';
}

int	normalizer(char **str)
{
	int		spaced_length;
	char	*normal_str;

	spaced_length = count_spaced_chars(*str);
	normal_str = malloc(spaced_length + 1);
	if (!normal_str)
		return (perror("Error: Malloc normalised string"), ERROR);
	copy_str_with_extra_spaces(*str, normal_str);
	free(*str);
	*str = normal_str;
	return (0);
}
