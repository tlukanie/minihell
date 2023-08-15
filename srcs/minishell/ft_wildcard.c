/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wildcard.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/09 18:51:22 by okraus            #+#    #+#             */
/*   Updated: 2023/08/15 19:44:16 by tlukanie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_update_lst(t_list **lst, char *s, int hidden)
{
	t_list	*leaf;

	if (s[0] == '.' && !hidden)
	{
		free(s);
		return ;
	}
	leaf = ft_lstnew(s);
	if (!*lst)
		*lst = leaf;
	else
		ft_lstadd_back(lst, leaf);
}

static t_list	*ft_get_dir(int mode)
{
	struct dirent	*dp;
	struct stat     statbuf;
	t_list			*lst;
	char			*s;
	DIR				*dir;

	dir = opendir(".");
	lst = NULL;
	if (dir == NULL)
	{
		perror("opendir() error");
		return NULL;
	}
	while ((dp = readdir(dir)) != NULL)
	{
		if (lstat(dp->d_name, &statbuf) == -1)
			continue;
		s = ft_stringcopy(dp->d_name);
		ft_update_lst(&lst, s, mode);
	}
	closedir(dir);
	return (lst);
}

static void	ft_delstring(void *ptr)
{
	char	*s;

	s = ptr;
	if (s)
		free(s);
	s = NULL;	
}

static void	ft_compare(char *s, t_list **dir)
{
	t_list	*lst;
	t_list	*plst;
	char	*w;

	lst = *dir;
	plst = NULL;
	while (lst)
	{
		w = lst->content;
		if (!ft_wild_fits(w, s))
		{
			if (plst)
			{
				plst->next = lst->next;
				ft_lstdelone(lst, ft_delstring);
				lst = plst->next;
			}
			else
			{
				*dir = lst->next;
				ft_lstdelone(lst, ft_delstring);
				lst = *dir;
			}			
		}
		else
		{
			plst = lst;
			lst = lst->next;
		}
	}
}

static void ft_replace_token_helper(t_list *lst, t_token *token, int mode)
{
	if (mode == 1)
	{
		lst->next = NULL;
		free(token->text);
		token->text = NULL;
		token->type = SPACETOKEN;
	}
	else
	{
		token->type = SPACETOKEN;
		token->text = NULL;
	}
}

static void	ft_replace_token(t_ms *ms, t_list *lst, t_token *token, t_list *dir)
{
	t_list *next;
	t_list *new;

	next = lst->next;
	ft_replace_token_helper(lst, token, 1);
	while (dir)
	{
		token = malloc(sizeof(t_token));
		if (!token)
			ft_exit(ms, 1);
		token->type = NOQUOTE;
		token->text = ft_stringcopy((char *)dir->content);
		new = ft_lstnew(token);
		if (!new)
			ft_exit(ms, 1);
		ft_lstadd_back(&lst, new);
		token = malloc(sizeof(t_token));
		if (!token)
			ft_exit(ms, 1);
		ft_replace_token_helper(lst, token, 2);
		new = ft_lstnew(token);
		if (!new)
			ft_exit(ms, 1);
		ft_lstadd_back(&lst, new);
		dir = dir->next;
	}
	ft_lstadd_back(&lst, next);
}

static void	ft_replace_wild(t_ms *ms, t_list *lst, t_token *token, char *s)
{
	t_list	*dir;

	if (s && s[0] != '.')
		dir = ft_get_dir(0);
	else
		dir = ft_get_dir(1);
	ft_compare(s, &dir);
	if (dir)
		ft_replace_token(ms, lst, token, dir);
	ft_lstclear(&dir, ft_delstring);
}

void	ft_expand_wild(t_ms *ms, t_list *lst)
{
	int		i;
	char	*s;
	t_token	*token;

	(void)ms;
	token = lst->content;
	if (token->type == NOQUOTE)
	{
		i = 0;
		s = token->text;
		while (s && s[i])
		{
			if (s[i] == '*')
			{
				ft_replace_wild(ms, lst, token, s);
				break;
			}
			i++;
		}
	}
}
