/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_envvar.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/14 15:54:16 by okraus            #+#    #+#             */
/*   Updated: 2023/07/19 16:04:30 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_putenv(t_list *el)
{
	t_list	*lst;
	t_ev	*ev;

	lst = el;
	while (lst)
	{
		if (lst->content)
		{
			ev = lst->content;
			ft_printf("%s\n", ev->s);
		}
		lst = lst->next;
	}
}

void	ft_changeenvvar(t_ms *ms, char *var, char *val)
{
	int		i;
	int		l;
	char	*s;

	i = 0;
	l = ft_strlen(var);
	while (ms->ev[i])
	{
		if (!ft_strncmp(ms->ev[i], var, l)
			&& ms->ev[i][l] == '=')
		{
			s = ft_strjoin(var, "=");
			s = ft_strjoin_freeleft(s, val);
			free(ms->ev[i]);
			ms->ev[i] = s;
			return ;
		}
		i++;
	}
	//add variable if not found?
}

int	ft_findenvvar(t_ms *ms, char *var)
{
	int		i;
	int		l;

	l = ft_strlen(var);
	i = 0;
	while (ms->ev[i])
	{
		if (!ft_strncmp(ms->ev[i], var, l)
			&& ms->ev[i][l] == '=' && ms->ev[i][l + 1])
		{
			return (i);
		}
		i++;
	}
	return (-1);
}

char	*ft_getenvval(t_ms *ms, char *var)
{
	int	i;
	int	l;

	i = 0;
	l = ft_strlen(var);
	while (ms->ev[i])
	{
		if (!ft_strncmp(ms->ev[i], var, l)
			&& ms->ev[i][l] == '=' && ms->ev[i][l + 1])
		{
			return (&ms->ev[i][l + 1]);
		}
		i++;
	}
	return (NULL);
}

char	*ft_getenvvar(char *s)
{
	int		i;
	int		l;
	char	*var;

	i = 0;
	if (!s)
		return (NULL);
	while (s[i] && s[i] != '=')
		i++;
	l = i;
	i = 0;
	var = ft_calloc(sizeof(char), l + 1);
	if (!var)
		return (NULL);
	while (s[i] && s[i] != '=')
	{
		var[i] = s[i];
		i++;
	}
	return (var);
}
