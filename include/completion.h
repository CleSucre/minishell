/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   completion.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpierrot <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/07 03:46:00 by mpierrot          #+#    #+#             */
/*   Updated: 2024/07/07 07:37:38 by mpierrot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPLETION_H
# define COMPLETION_H

// ########################################################
// #					TAB_COMPLETION					  #
// ########################################################

int		tab_completion(t_minishell *minishell, char **input);

// ########################################################
// #						DICTIONNARY					  #
// ########################################################

int		search_in_path(t_dict *dict, char *odir);
int		creation_dict(t_minishell *minishell);



// ########################################################
// #							BST						  #
// ########################################################

t_dict	*insert_node(t_dict *root, char *key, char *value);
t_dict	*create_node(char *key, char *value);
t_dict	*search_node(t_dict *root, char *key);
t_dict	*cut_node(t_dict *root, char *key);
void	free_node(t_dict *node);
void	free_branch(t_dict *root);
void	print_branch(t_dict *root);

#endif