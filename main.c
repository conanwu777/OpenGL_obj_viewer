/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwu <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/10 00:33:30 by cwu               #+#    #+#             */
/*   Updated: 2018/10/10 00:34:19 by cwu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

float	g_v[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, -2, 1};
bool	g_spin = 1;
int		g_sw = 0;
float	g_theta = 0;
float	g_rotx = 0;
float	g_rotz = 0;
t_data	g_data;
t_bufs	g_bufs;
t_mat	g_mat;

void	processinput(GLFWwindow *window, int k, int s, int a, int m)
{
	(void)k;
	(void)s;
	(void)a;
	(void)m;
	if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		g_spin = !g_spin;
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		g_sw = (g_sw % 300) + 1;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void	keyholds(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		g_v[12] -= 0.02;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		g_v[12] += 0.02;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		g_v[13] -= 0.02;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		g_v[13] += 0.02;
	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
		g_v[14] += 0.02;
	if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
		g_v[14] -= 0.02;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		g_rotx -= 0.02;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		g_rotx += 0.02;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		g_rotz -= 0.02;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		g_rotz += 0.02;
}

void	loop(GLFWwindow *window)
{
	g_sw += (g_sw % 100 != 0 ? 1 : 0);
	keyholds(window);
	getmatrix(g_theta, &g_mat, g_rotx, g_rotz);
	glUniformMatrix4fv(g_bufs.proj, 1, GL_FALSE, g_mat.prod);
	glUniform1i(g_bufs.sw, g_sw);
	glClearColor(0.06f, 0.09f, 0.15f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, g_bufs.tex);
	glUseProgram(g_bufs.shaders);
	glBindVertexArray(g_bufs.vao);
	glDrawElements(GL_TRIANGLES, 3 * g_data.nf, GL_UNSIGNED_INT, 0);
	glfwSwapBuffers(window);
	glfwPollEvents();
	if (g_spin)
		g_theta += PI / 100;
	if (g_theta > 2 * PI)
		g_theta -= 2 * PI;
}

void	quit(int i)
{
	if (i == 1)
		printf("%sError: Unable to open .obj file\n", RED);
	if (i == 2)
		printf("%sError: Unable to open .bmp file\n", RED);
	if (i == 3)
		printf("%sError: Wrong .obj file format\n", RED);
	exit(1);
}

int		main(int ac, char **av)
{
	GLFWwindow *window;

	if (ac != 3)
		return (printf("%sUsage: ./scop [file.obj] [texture.bmp]\n", ORANGE));
	if (!parse(&g_data, av[1], 0))
		return (-1);
	window = createwindow(av[2]);
	glfwSetKeyCallback(window, processinput);
	while (!glfwWindowShouldClose(window))
		loop(window);
	glDeleteVertexArrays(1, &g_bufs.vao);
	glDeleteBuffers(1, &g_bufs.vbo);
	glDeleteBuffers(1, &g_bufs.ebo);
	glDeleteProgram(g_bufs.shaders);
	glfwDestroyWindow(window);
	glfwTerminate();
	return (0);
}
