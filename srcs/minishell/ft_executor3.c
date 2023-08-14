/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_executor3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlukanie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 14:16:12 by tlukanie          #+#    #+#             */
/*   Updated: 2023/08/14 14:16:16 by tlukanie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_pathjoin(char *path, char *cmd)
{
	char	*str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	str = ft_calloc(ft_strlen(path) + 2 + ft_strlen(cmd), sizeof(char));
	while (path[i])
	{
		str[i] = path[i];
		i++;
	}
	str[i] = '/';
	i++;
	while (cmd[j])
	{
		str[i + j] = cmd[j];
		j++;
	}
	return (str);
}

void	ft_closepipes(t_ms *ms, int i)
{
	int	k;

	k = 0;
	if (ms->cs[i].ctn > 1)
	{
		while (k < ms->cs[i].ctn - 1)
		{
			close(ms->cs[i].pipes[k][0]);
			close(ms->cs[i].pipes[k][1]);
			k++;
		}
	}
	k = 0;
	while (k < ms->cs[i].ctn)
	{
		if (ms->cs[i].ct[k].hd)
		{
			if (ms->cs[i].ct[k].hdpipe[0] > 2)
				close(ms->cs[i].ct[k].hdpipe[0]);
			if (ms->cs[i].ct[k].hdpipe[1] > 2)
				close(ms->cs[i].ct[k].hdpipe[1]);
		}
		k++;
	}
}

void	ft_closefds(t_ms *ms, int i)
{
	int	k;

	k = 0;
	while (k < ms->cs[i].ctn)
	{
		if (ms->cs[i].ct[k].fds[0][0] > 2)
			if (close (ms->cs[i].ct[k].fds[0][0]))
				printf("Error closing input %i\n", ms->cs[i].ct[k].fds[0][0]);
		if (ms->cs[i].ct[k].fds[1][0] > 2)
			if (close (ms->cs[i].ct[k].fds[1][0]))
				printf("Error closing output %i\n", ms->cs[i].ct[k].fds[1][0]);
		if (ms->cs[i].ct[k].fds[2][0] > 2)
			if (close (ms->cs[i].ct[k].fds[2][0]))
				printf("Error closing err %i\n", ms->cs[i].ct[k].fds[2][0]);
		k++;
	}
}

int	ft_exec(t_ms *ms, char **cmd)
{
	int		i;
	int		fail;
	char	*cmdp;
	char	**paths;
	char	**env;

	i = 0;
	fail = 1;
	env = ft_list2split(ms->el);
	if ((ft_strchr(cmd[0], '/')) && access(cmd[0], X_OK) == 0)
		execve(cmd[0], cmd, env);
	paths = ft_getenvvals(ms, "PATH");
	if (!paths)
		return (fail);
	while (paths[i])
	{
		cmdp = ft_pathjoin(paths[i], cmd[0]);
		if (access(cmdp, X_OK) == 0)
			execve(cmdp, cmd, env);
		free(cmdp);
		i++;
	}
	ft_printf_fd(2, "%s: command not found\n", cmd[0]);
	ft_free_split(&env);
	return (fail);
}
