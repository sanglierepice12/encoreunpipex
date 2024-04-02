/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gostr <gostr@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 16:53:28 by gostr             #+#    #+#             */
/*   Updated: 2024/04/02 15:37:07 by gsuter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

void	ft_init_cmd(t_struct *var)
{
	size_t	i;
	char	*temp;
	char	*path;

	i = 0;
	while (var->path[i])
	{
		temp = ft_strjoin(var->path[i], "/");
		path = ft_strjoin(temp, var->cmd[0]);
		free(temp);
		if (access(path, F_OK) == 0)
		{
			var->cmd[0] = path;
			break;
		}
		free(path);
		i++;
	}
}

void	ft_init_path(t_struct *var, char **env)
{
	size_t	i;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5))
		i++;
	if (!env[i])
	{
		var->path = ft_calloc(1, sizeof(char *));
		return ;
	}
	else
		var->path = ft_split(env[i] + 5, ':');
}

int	main(int arc, char **argv, char **env)
{
	t_struct	*var;
	if (arc != 5)
		return (ft_printf("Not enough arguments\n"), EXIT_FAILURE);
	var = ft_calloc(1, sizeof(t_struct));
	if (!var)
		return (EXIT_FAILURE);
	ft_init_path(var, env);
	ft_process(var, argv);
	ft_free_pipex(var);
}