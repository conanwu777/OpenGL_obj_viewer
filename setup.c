/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/09 22:40:20 by cwu               #+#    #+#             */
/*   Updated: 2018/10/09 22:40:22 by cwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

GLFWwindow	*createwindow(char *texname)
{
	GLFWwindow *window;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	window = glfwCreateWindow(1200, 1200, "scop", NULL, NULL);
	glfwMakeContextCurrent(window);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	initbufmatrix(&g_bufs, &g_mat, texname);
	buffervertexsetup(&g_bufs, &g_data);
	return (window);
}

char		*filetostr(char *filename)
{
	char	buf[1000];
	char	*ret;
	FILE	*ptr;
	int		i;

	ret = calloc(1000, 1);
	ptr = fopen(filename, "r");
	i = 0;
	while (fgets(buf, 999, ptr))
	{
		ret = realloc(ret, strlen(ret) + strlen(buf) + 1);
		strcpy(ret + i, buf);
		i += strlen(buf);
	}
	fclose(ptr);
	return (ret);
}

void		getshader(t_bufs *g_bufs)
{
	t_uint				vertexshader;
	t_uint				fragmentshader;
	const GLchar *const	vsrc = filetostr("first.vs");
	const GLchar *const	fsrc = filetostr("first.fs");

	vertexshader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexshader, 1, &vsrc, NULL);
	glCompileShader(vertexshader);
	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentshader, 1, &fsrc, NULL);
	glCompileShader(fragmentshader);
	g_bufs->shaders = glCreateProgram();
	glAttachShader(g_bufs->shaders, vertexshader);
	glAttachShader(g_bufs->shaders, fragmentshader);
	glLinkProgram(g_bufs->shaders);
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);
	free(vsrc);
	free(fsrc);
}

void		completedata(t_data *g_data, int i, int j)
{
	g_data->color = calloc(3 * g_data->nv, sizeof(float));
	while (++i < g_data->nv)
	{
		j = -1;
		while (++j < 3)
			g_data->color[3 * i + j] = (g_data->pos[3 * i + j] + 1.2) / 2.4;
	}
	g_data->out = calloc(8 * g_data->nv, sizeof(float));
	i = -1;
	while (++i < g_data->nv)
	{
		j = -1;
		while (++j < 3)
		{
			g_data->out[8 * i + j] = g_data->pos[3 * i + j];
			g_data->out[8 * i + j + 3] = g_data->color[3 * i + j];
		}
		g_data->out[8 * i + 6] = atan(
			(fabsf(g_data->pos[3 * i]) > fabsf(g_data->pos[3 * i + 2]) ?
				(g_data->pos[3 * i + 2] / g_data->pos[3 * i]) :
				g_data->pos[3 * i] / g_data->pos[3 * i + 2])) / PI;
		g_data->out[8 * i + 7] = g_data->color[3 * i + 1];
	}
}

void		buffervertexsetup(t_bufs *g_bufs, t_data *g_data)
{
	completedata(g_data, -1, 0);
	glBindVertexArray(g_bufs->vao);
	glBindBuffer(GL_ARRAY_BUFFER, g_bufs->vbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * g_data->nv * sizeof(float),
		g_data->out, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
		8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
		8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_bufs->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * g_data->nf * sizeof(int),
		g_data->ind, GL_STATIC_DRAW);
}
