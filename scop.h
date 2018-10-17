/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/10 00:37:48 by cwu               #+#    #+#             */
/*   Updated: 2018/10/10 00:38:12 by cwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCOP_H
# define SCOP_H

# include <GLFW/glfw3.h>
# include <stdlib.h>
# include <stdbool.h>
# include <stdio.h>
# include <string.h>
# include <math.h>
# include <fcntl.h>

# define ORANGE "\e[1;38;2;255;120;10m"
# define RED "\e[1;38;2;225;20;20m"
# define WHITESPACE " \t\n\r"
# define PI 3.1415926
# define FAR 50
# define NEAR 0.1

typedef unsigned int	t_uint;

typedef struct			s_data
{
	float	*pos;
	float	*color;
	float	*out;
	int		*ind;
	int		nv;
	int		nf;
	float	max;
}						t_data;

typedef struct			s_mat
{
	float	*m;
	float	*p;
	float	*v;
	float	*camx;
	float	*camz;
	float	*prod;
}						t_mat;

typedef struct			s_buffers
{
	t_uint	vao;
	t_uint	vbo;
	t_uint	ebo;
	t_uint	tex;
	t_uint	proj;
	t_uint	sw;
	t_uint	shaders;
}						t_bufs;

typedef struct			s_pt
{
	float x;
	float y;
	float z;
}						t_pt;

typedef struct			s_tex
{
	unsigned char	*img;
	int				w;
	int				h;
	int				l;
}						t_tex;

extern	bool			g_spin;
extern	int				g_sw;
extern	float			g_v[16];
extern	t_data			g_data;
extern	t_bufs			g_bufs;
extern	t_mat			g_mat;
extern	float			g_theta;

GLFWwindow				*createwindow(char *texname);
char					*filetostr(char *filename);
void					getshader(t_bufs *bufs);
void					buffervertexsetup(t_bufs *bufs, t_data *data);
void					mult(float *a, float *b, float *p);
void					initbufmatrix(t_bufs *bufs, t_mat *mat, char *texname);
void					getmatrix(float theta, t_mat *mat,
float rotx, float rotz);
int						parse(t_data *data, char *filename, unsigned long ln);
void					loadbmp(t_tex *texture, char *filename);
void					quit(int i);

#endif
