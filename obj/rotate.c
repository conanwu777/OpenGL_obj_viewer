/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/11 13:11:23 by cwu               #+#    #+#             */
/*   Updated: 2018/10/11 13:11:25 by cwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void	parsevert(char *p)
{
	int		i;
	float	t[3];

	i = -1;
	while (++i < 3)
	{
		p = strchr(p, ' ') + 1;
		t[i] = atof(p);
	}
	printf("v %f %f %f\n", t[0], 0 - t[1], t[2]);
}

int		main(void)
{
	FILE			*file;
	char			*p;
	unsigned long	ln;
	static char		*line = NULL;

	ln = 0;
	if (!(file = fopen("nymph1.obj", "r")))
		exit(1);
	while (getline(&line, &ln, file) != -1)
	{
		p = line;
		if (line[0] == 'v' && line[1] == ' ')
			parsevert(p);
		else
			printf("%s", line);
	}
	fclose(file);
	return (1);
}
