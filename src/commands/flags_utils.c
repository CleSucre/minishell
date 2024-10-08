/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julthoma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 08:30:00 by julthoma          #+#    #+#             */
/*   Updated: 2024/07/05 08:30:00 by julthoma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief Check if a flag is present in the argv
 *
 * @param char **argv Arguments
 * @param char flag Flag to search
 * @return int 1 if the flag is present, 0 otherwise
 */
int	contain_flag(char **argv, char flag)
{
	int	i;
	int	j;

	i = 0;
	while (argv[i])
	{
		if (argv[i][0] == '-')
		{
			j = 1;
			while (argv[i][j])
			{
				if (argv[i][j] == flag)
					return (1);
				j++;
			}
		}
		i++;
	}
	return (0);
}
