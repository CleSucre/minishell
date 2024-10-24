/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal_cursor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 03:48:00 by mpierrot          #+#    #+#             */
/*   Updated: 2024/06/14 03:48:00 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Move cursor backward from cols to n positions
 *
 * @param size_t n
 */
void	move_cursor_back(size_t n)
{
	ft_printf(CURSOR_LEFT, n);
}

/**
 * @brief Move cursor forward from cols to n positions
 *
 * @param size_t n
 */
void	move_cursor_forward(size_t n)
{
	ft_printf(CURSOR_RIGHT, n);
}
