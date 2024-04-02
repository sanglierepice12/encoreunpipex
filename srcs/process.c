/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gostr <gostr@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 10:06:44 by gostr             #+#    #+#             */
/*   Updated: 2024/04/02 10:06:44 by gostr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

void	ft_error(t_struct *var)
{
	ft_free_tab(var->path);
	ft_free_tab(var->cmd);
	free(var);
}

static void	ft_second_child_process(t_struct *var, char **argv, char **env)
{
	close(var->pipe_fd[1]);
	var->fd[1] = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (var->fd[1] == -1)
	{
		perror("Wrong fd[1]\n");
		exit(EXIT_FAILURE);
	}
	var->cmd = ft_split(argv[3], 32);
	if (!var->cmd)
		exit(EXIT_FAILURE);
	ft_init_cmd(var);
	if (dup2(var->pipe_fd[0], STDIN_FILENO) == -1)
	{
		perror("Error dup2\n");
		exit(EXIT_FAILURE);
	}
	if (dup2(var->fd[1], STDOUT_FILENO) == -1)
	{
		perror("Error dup2\n");
		exit(EXIT_FAILURE);
	}
	close(var->pipe_fd[0]);
	close(var->fd[1]);
	execve(var->cmd[0], var->cmd, env);
	ft_error(var);
	perror("Execve second command failed\n");
	exit(EXIT_FAILURE);
}

static void	ft_child_process(t_struct *var, char **argv, char **env)
{
	close(var->pipe_fd[0]);
	var->fd[0] = open(argv[1], O_RDONLY);
	if (var->fd[0] == -1)
	{
		perror("Wrong fd[0]\n");
		exit(EXIT_FAILURE);
	}
	var->cmd = ft_split(argv[2], 32);
	if (!var->cmd)
		exit(EXIT_FAILURE);
	ft_init_cmd(var);
	if (dup2(var->fd[0], STDIN_FILENO) == -1)
	{
		perror("Error dup2\n");
		exit(EXIT_FAILURE);
	}
	if (dup2(var->pipe_fd[1], STDOUT_FILENO) == -1)
	{
		perror("Error dup2\n");
		exit(EXIT_FAILURE);
	}
	close(var->pipe_fd[1]);
	close(var->fd[0]);
	execve(var->cmd[0], var->cmd, env);
	ft_error(var);
	perror("Execve first command failed\n");
	exit(EXIT_FAILURE);
}

void	ft_process(t_struct *var, char **argv, char **env)
{
	if (pipe(var->pipe_fd) == -1)
	{
		perror("Pipe error\n");
		ft_error(var);
		exit(EXIT_FAILURE);
	}
	var->pid[0] = fork();
	if (var->pid[0] == -1)
	{
		perror("Pid error\n");
		ft_error(var);
		exit(EXIT_FAILURE);
	}
	if (!var->pid[0])
		ft_child_process(var, argv, env);
	//
	var->pid[1] = fork();
	if (var->pid[1] == -1)
	{
		perror("Pid error\n");
		ft_error(var);
		exit(EXIT_FAILURE);
	}
	if (!var->pid[1])
		ft_second_child_process(var, argv, env);
	close(var->pipe_fd[0]);
	close(var->pipe_fd[1]);
	waitpid(var->pid[0], 0, 0);
	waitpid(var->pid[1], 0, 0);
}