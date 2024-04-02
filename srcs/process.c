/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gostr <gostr@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 10:06:44 by gostr             #+#    #+#             */
/*   Updated: 2024/04/02 15:28:49 by gsuter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

void	ft_free_pipex(t_struct *var)
{
	if (var->path)
		ft_free_tab(var->path);
	if (var->cmd)
		ft_free_tab(var->cmd);
	free(var);
}

void	pipex_exit(t_struct *var)
{
	perror("pipex");
	ft_free_pipex(var);
	exit(EXIT_FAILURE);
}

static void	ft_second_child_process(t_struct *var, char **argv)
{
	close(var->pipe_fd[1]);
	var->fd[1] = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (var->fd[1] == -1)
		pipex_exit(var);
	var->cmd = ft_split(argv[3], 32);
	if (var->cmd)
		ft_init_cmd(var);
	if (dup2(var->pipe_fd[0], STDIN_FILENO) == -1)
		pipex_exit(var);
	if (dup2(var->fd[1], STDOUT_FILENO) == -1)
		pipex_exit(var);
	close(var->pipe_fd[0]);
	close(var->fd[1]);
	if (var->cmd)
		execve(var->cmd[0], var->cmd, var->path);
	pipex_exit(var);
}

static void	ft_child_process(t_struct *var, char **argv)
{
	close(var->pipe_fd[0]);
	var->fd[0] = open(argv[1], O_RDONLY);
	if (var->fd[0] == -1)
		pipex_exit(var);
	var->cmd = ft_split(argv[2], 32);
	if (var->cmd)
		ft_init_cmd(var);
	if (dup2(var->fd[0], STDIN_FILENO) == -1)
		pipex_exit(var);
	if (dup2(var->pipe_fd[1], STDOUT_FILENO) == -1)
		pipex_exit(var);
	close(var->pipe_fd[1]);
	close(var->fd[0]);
	if (var->cmd)
		execve(var->cmd[0], var->cmd, var->path);
	pipex_exit(var);
}

void	ft_process(t_struct *var, char **argv)
{
	if (pipe(var->pipe_fd) == -1)
		pipex_exit(var);
	var->pid[0] = fork();
	if (var->pid[0] == -1)
		pipex_exit(var);
	if (!var->pid[0])
		ft_child_process(var, argv);
	var->pid[1] = fork();
	if (var->pid[1] == -1)
		pipex_exit(var);
	if (!var->pid[1])
		ft_second_child_process(var, argv);
	close(var->pipe_fd[0]);
	close(var->pipe_fd[1]);
	waitpid(var->pid[0], 0, 0);
	waitpid(var->pid[1], 0, 0);
}