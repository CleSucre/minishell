/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 17:13:11 by julthoma          #+#    #+#             */
/*   Updated: 2023/11/13 04:21:20 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*res;

	if (!lst || !f || !del)
		return (NULL);
	res = ft_lstnew(f(lst->content));
	if (res == NULL)
	{
		ft_lstclear(&res, del);
		return (NULL);
	}
	res->next = ft_lstmap(lst->next, f, del);
	return (res);
}
