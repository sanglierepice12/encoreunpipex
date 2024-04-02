/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gostr <gostr@student.1337.ma>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 16:53:28 by gostr             #+#    #+#             */
/*   Updated: 2024/03/30 16:53:28 by gostr            ###   ########.fr       */
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

	if (!env)
		return (var->path = NULL, (void)0);
	i = 0;
	while(ft_strncmp(env[i], "PATH=", 5))
		i++;
	var->path = ft_split(env[i] + 5, ':');
	if (!var->path)
		exit(EXIT_FAILURE);
}

int	main(int arc, char **argv, char **env)
{
	t_struct	*var;
	if (arc != 5)
		return (ft_printf("Not enough arguments\n"), EXIT_FAILURE);
	var = malloc(sizeof(t_struct) * 1);
	if (!var)
		return (printf("Malloc crash\nw	"), EXIT_FAILURE);
	ft_init_path(var, env);
	ft_process(var, argv, env);
	ft_free_tab(var->path);
	free(var);
}