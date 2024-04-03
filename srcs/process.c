/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gostr <gostr@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 10:06:44 by gostr             #+#    #+#             */
/*   Updated: 2024/04/02 16:51:58 by gsuter           ###   ########.fr       */
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

void	pipex_exit(t_struct *var, int flag)
{
	if (flag == 1)
	{
		close(var->pipe_fd[0]);
		close(var->fd[1]);
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
	}
	else if (flag == 0)
	{
		close(var->pipe_fd[1]);
		close(var->fd[0]);
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
	}
	else
	{
		close(var->pipe_fd[0]);
		close(var->pipe_fd[1]);
	}
	perror("pipex");
	if (flag != 2)
	{
		ft_free_pipex(var);
		exit(EXIT_FAILURE);
	}
}

static void	ft_second_child_process(t_struct *var, char **argv)
{
	close(var->pipe_fd[1]);
	var->fd[1] = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (var->fd[1] == -1)
		pipex_exit(var, 1);
	argv[3] = ft_space_cmd(argv[3]);
	var->cmd = ft_split(argv[3], 32);
	if (var->cmd)
	{
		ft_init_cmd(var);
		if (access(var->cmd[0], F_OK) && access(var->cmd[0], X_OK))
			pipex_exit(var, 1);
	}
	if (dup2(var->pipe_fd[0], STDIN_FILENO) == -1)
		pipex_exit(var, 2);
	if (dup2(var->fd[1], STDOUT_FILENO) == -1)
		pipex_exit(var, 2);
	if (var->cmd)
	{
		close(var->pipe_fd[0]);
		close(var->fd[1]);
		execve(var->cmd[0], var->cmd, var->path);
	}
	errno = 2;
	pipex_exit(var, 1);
}

static void	ft_child_process(t_struct *var, char **argv)
{
	close(var->pipe_fd[0]);
	var->fd[0] = open(argv[1], O_RDONLY);
	if (var->fd[0] == -1)
		pipex_exit(var, 0);
	argv[2] = ft_space_cmd(argv[2]);
	var->cmd = ft_split(argv[2], 32);
	if (var->cmd)
	{
		ft_init_cmd(var);
		if (access(var->cmd[0], F_OK) && access(var->cmd[0], X_OK))
			pipex_exit(var, 0);
	}
	if (dup2(var->fd[0], STDIN_FILENO) == -1)
		pipex_exit(var, 2);
	if (dup2(var->pipe_fd[1], STDOUT_FILENO) == -1)
		pipex_exit(var, 2);
	if (var->cmd)
	{
		close(var->pipe_fd[1]);
		close(var->fd[0]);
		execve(var->cmd[0], var->cmd, var->path);
	}
	errno = 2;
	pipex_exit(var, 0);
}

void	ft_process(t_struct *var, char **argv)
{
	if (pipe(var->pipe_fd) == -1)
		pipex_exit(var, -1);
	var->pid[0] = fork();
	if (var->pid[0] == -1)
		pipex_exit(var, -1);
	if (!var->pid[0])
		ft_child_process(var, argv);
	var->pid[1] = fork();
	if (var->pid[1] == -1)
		pipex_exit(var, -1);
	if (!var->pid[1])
		ft_second_child_process(var, argv);
	close(var->pipe_fd[0]);
	close(var->pipe_fd[1]);
	waitpid(var->pid[0], 0, 0);
	waitpid(var->pid[1], 0, 0);
}
