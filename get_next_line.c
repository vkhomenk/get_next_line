/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkhomenk <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/22 13:56:44 by vkhomenk          #+#    #+#             */
/*   Updated: 2019/02/03 20:51:23 by vkhomenk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static t_list	*def_list(t_list **save, int fd)
{
	t_list	*tmp;

	tmp = *save;
	while (tmp)
	{
		if (tmp->content_size == (size_t)fd)
			return (tmp);
		tmp = tmp->next;
	}
	if (!(tmp = ft_lstnew("", 1)))
		return (NULL);
	ft_lstadd(save, tmp);
	tmp->content_size = fd;
	return (tmp);
}

static int		fill_line(t_list *lst, char **line)
{
	char	*c;
	size_t	i;

	if (!ft_strchr(lst->content, '\n'))
		return (0);
	c = (char*)lst->content;
	i = 0;
	while (c[i] != '\n')
		i++;
	MOCHECK(!(*line = ft_strsub(lst->content, 0, i++)));
	ft_memmove(lst->content, &c[i], ft_strlen(&c[i]) + 1);
	return (1);
}

int				get_next_line(const int fd, char **line)
{
	static t_list	*save;
	char			buf[BUFF_SIZE + 1];
	char			*str;
	t_list			*lst;
	int				cr;

	MOCHECK(!line || fd < 0 || BUFF_SIZE < 1 || read(fd, buf, 0) ||
	!(lst = def_list(&save, fd)));
	while (1)
	{
		if ((cr = fill_line(lst, line)))
			return (cr);
		MOCHECK((cr = read(fd, buf, BUFF_SIZE)) < 0);
		if (cr == 0 && (*(char*)lst->content))
			buf[cr++] = '\n';
		else if (cr == 0)
			return (0);
		buf[cr] = 0;
		MOCHECK(!(str = ft_strjoin((char*)lst->content, buf)));
		ft_memdel(&lst->content);
		lst->content = str;
	}
}
