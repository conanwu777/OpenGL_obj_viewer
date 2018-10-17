/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/09 23:38:42 by cwu               #+#    #+#             */
/*   Updated: 2018/10/09 23:41:33 by cwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	mult(float *a, float *b, float *p)
{
	int i;
	int j;
	int k;

	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
		{
			p[i * 4 + j] = 0;
			k = -1;
			while (++k < 4)
				p[i * 4 + j] += a[i * 4 + k] * b[k * 4 + j];
		}
	}
}

void	inittexture(t_bufs *g_bufs, char *texname)
{
	int				db;
	t_tex			tex;

	glGenTextures(1, &g_bufs->tex);
	loadbmp(&tex, texname);
	glBindTexture(GL_TEXTURE_2D, g_bufs->tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex.w, tex.h, 0, GL_RGB,
		GL_UNSIGNED_BYTE, tex.img);
	glGenerateMipmap(GL_TEXTURE_2D);
	free(tex.img);
}

void	loadbmp(t_tex *texture, char *filename)
{
	FILE			*file;
	int				size;
	unsigned char	c;
	int				i;

	if ((file = fopen(filename, "r")) == NULL)
		quit(2);
	fseek(file, 18, SEEK_SET);
	fread(&texture->w, 4, 1, file);
	fread(&texture->h, 4, 1, file);
	texture->l = texture->w * 3;
	texture->w < 0 ? texture->w = -texture->w : 0;
	texture->h < 0 ? texture->h = -texture->h : 0;
	size = texture->l * texture->h;
	texture->img = calloc(size + 1, 1);
	fseek(file, 54, SEEK_SET);
	fread(texture->img, 1, texture->l * texture->h, file);
	i = -1;
	while (++i < size / 3)
	{
		c = texture->img[3 * i];
		texture->img[3 * i] = texture->img[3 * i + 2];
		texture->img[3 * i + 2] = c;
	}
	fclose(file);
}

void	initbufmatrix(t_bufs *g_bufs, t_mat *g_mat, char *texname)
{
	static float m[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
	static float p[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		-FAR / (FAR - NEAR), -1, 0, 0, -FAR * NEAR / (FAR - NEAR), 0};

	p[0] = 1.0 / tan(90 / 2 * PI / 180);
	p[5] = p[0];
	glGenVertexArrays(1, &g_bufs->vao);
	glGenBuffers(1, &g_bufs->vbo);
	glGenBuffers(1, &g_bufs->ebo);
	getshader(g_bufs);
	g_bufs->proj = glGetUniformLocation(g_bufs->shaders, "p");
	g_bufs->sw = glGetUniformLocation(g_bufs->shaders, "sw");
	inittexture(g_bufs, texname);
	g_mat->m = calloc(16, sizeof(float));
	g_mat->m = memcpy(g_mat->m, &m[0], 16 * sizeof(float));
	g_mat->p = calloc(16, sizeof(float));
	g_mat->p = memcpy(g_mat->p, &p[0], 16 * sizeof(float));
	g_mat->v = calloc(16, sizeof(float));
	g_mat->v = memcpy(g_mat->v, &g_v[0], 16 * sizeof(float));
	g_mat->camx = calloc(16, sizeof(float));
	g_mat->camx = memcpy(g_mat->camx, &m[0], 16 * sizeof(float));
	g_mat->camz = calloc(16, sizeof(float));
	g_mat->camz = memcpy(g_mat->camz, &m[0], 16 * sizeof(float));
	g_mat->prod = calloc(16, sizeof(float));
}

void	getmatrix(float theta, t_mat *g_mat, float rotx, float rotz)
{
	float t[16];
	float t2[16];
	float t3[16];

	g_mat->m[0] = cos(theta);
	g_mat->m[2] = sin(theta);
	g_mat->m[8] = -sin(theta);
	g_mat->m[10] = cos(theta);
	g_mat->camx[5] = cos(rotx);
	g_mat->camx[6] = sin(rotx);
	g_mat->camx[9] = -sin(rotx);
	g_mat->camx[10] = cos(rotx);
	g_mat->camz[0] = cos(rotz);
	g_mat->camz[2] = sin(rotz);
	g_mat->camz[8] = -sin(rotz);
	g_mat->camz[10] = cos(rotz);
	mult(g_mat->camz, g_mat->camx, &t3[0]);
	mult(g_v, &t3[0], &t2[0]);
	mult(&t2[0], g_mat->p, &t[0]);
	mult(g_mat->m, &t[0], g_mat->prod);
}
