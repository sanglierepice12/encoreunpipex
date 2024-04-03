/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gostr <gostr@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/15 11:45:05 by gostr             #+#    #+#             */
/*   Updated: 2024/03/18 12:30:24 by gostr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../libft/incs/libft.h"
# include <fcntl.h>
# include <sys/wait.h>
# include <errno.h>

/* ------- PRINTF -------*/
# include <stdio.h>
/* ------- ------ -------*/

/* ------- STRUCT -------*/
typedef struct s_struct
{
	char	**path;
	int		fd[2];
	int		pipe_fd[2];
	pid_t	pid[2];
	char	**cmd;
}				t_struct;

/* ------- PIPEX -------*/
char	*ft_space_cmd(char *cmd);
void	ft_init_cmd(t_struct *var);

/* ------- PROCESS -------*/
void	ft_process(t_struct *var, char **argv);
void	ft_free_pipex(t_struct *var);
void	pipex_exit(t_struct *var, int flag);

/* ------- INIT -------*/

#endif