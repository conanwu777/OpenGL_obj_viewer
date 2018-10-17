/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/10 00:01:54 by cwu               #+#    #+#             */
/*   Updated: 2018/10/10 00:01:56 by cwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	parsevert(t_data *g_data, char **p)
{
	int i;

	g_data->pos = realloc(g_data->pos, sizeof(float) * (g_data->nv + 1) * 3);
	i = -1;
	while (++i < 3)
	{
		*p = strchr(*p, ' ') + 1;
		g_data->pos[3 * g_data->nv + i] = atof(*p);
	}
	g_data->nv++;
}

void	parseface(t_data *g_data, char *p)
{
	int i;

	g_data->ind = realloc(g_data->ind, sizeof(int) * (g_data->nf + 1) * 3);
	i = -1;
	while (++i < 3)
	{
		p = strchr(p, ' ') + 1;
		g_data->ind[3 * g_data->nf + i] = atoi(p) - 1;
	}
	g_data->nf++;
	if ((p = strchr(p, ' ')))
	{
		p++;
		g_data->ind = realloc(g_data->ind, sizeof(int) * (g_data->nf + 1) * 3);
		g_data->ind[3 * g_data->nf] = g_data->ind[3 * g_data->nf - 3];
		g_data->ind[3 * g_data->nf + 1] = g_data->ind[3 * g_data->nf - 1];
		g_data->ind[3 * g_data->nf + 2] = atoi(p) - 1;
		g_data->nf++;
	}
}

void	normalize(float mul, int i, t_pt max, t_pt min)
{
	while (++i < g_data.nv)
	{
		max.x = g_data.pos[3 * i] > max.x ? g_data.pos[3 * i] : max.x;
		min.x = g_data.pos[3 * i] < min.x ? g_data.pos[3 * i] : min.x;
		max.y = g_data.pos[3 * i + 1] > max.y ? g_data.pos[3 * i + 1] : max.y;
		min.y = g_data.pos[3 * i + 1] < min.y ? g_data.pos[3 * i + 1] : min.y;
		max.z = g_data.pos[3 * i + 2] > max.z ? g_data.pos[3 * i + 2] : max.z;
		min.z = g_data.pos[3 * i + 2] < min.z ? g_data.pos[3 * i + 2] : min.z;
	}
	min.x = (max.x + min.x) / 2;
	min.y = (max.y + min.y) / 2;
	min.z = (max.z + min.z) / 2;
	mul = max.x - min.x >= max.y - min.y ? max.x - min.x : max.y - min.y;
	mul = max.z - min.z > mul ? max.z - min.z : mul;
	i = -1;
	while (++i < g_data.nv)
	{
		g_data.pos[3 * i] = (g_data.pos[3 * i] - min.x) / mul;
		g_data.pos[3 * i + 1] = (g_data.pos[3 * i + 1] - min.y) / mul;
		g_data.pos[3 * i + 2] = (g_data.pos[3 * i + 2] - min.z) / mul;
	}
}

int		parse(t_data *g_data, char *filename, unsigned long ln)
{
	static char	*line = NULL;
	FILE		*file;
	char		*p;
	t_pt		max;

	bzero(g_data, sizeof(t_data));
	if (!(file = fopen(filename, "r")))
		quit(1);
	while (getline(&line, &ln, file) != -1)
	{
		p = line;
		if (line[0] == 'v' && line[1] == ' ')
			parsevert(g_data, &p);
		else if (line[0] == 'f' && line[1] == ' ')
			parseface(g_data, p);
	}
	if (g_data->nv == 0 || g_data->nf == 0)
		quit(3);
	free(line);
	fclose(file);
	max.x = g_data->pos[0];
	max.y = g_data->pos[1];
	max.z = g_data->pos[2];
	normalize(1.0, -1, max, max);
	return (1);
}
