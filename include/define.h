/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/28 12:24:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/05/28 12:24:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINE_H
# define DEFINE_H

# define HISTORY_FILE ".ministory"

// ########################################################
// #					CURSOR MOVES					  #
// ########################################################

# define CURSOR_UP "\033[{1}A"
# define CURSOR_DOWN "\033[{1}B"
# define CURSOR_LEFT "\033[{1}D"
# define CURSOR_RIGHT "\033[{1}C"
# define CURSOR_NLINE "\033[E"
# define CURSOR_PLINE "\033[F]"
# define CURSOR_ERA "\033[2K"

#endif