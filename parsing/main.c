#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Fonction pour tokenizer une chaîne de caractères avec gestion des parenthèses
char **tokenize(char *input) {
	int bufsize = 64;
	int position = 0;
	char **tokens = malloc(bufsize * sizeof(char *));
	char *token = NULL;
	int i = 0;
	char current_char;

	if (!tokens) {
		fprintf(stderr, "Erreur d'allocation\n");
		exit(EXIT_FAILURE);
	}

	while ((current_char = input[i]) != '\0') {
		// Ignorer les espaces
		if (isspace(current_char)) {
			i++;
			continue;
		}

		// Gérer les parenthèses comme des tokens séparés
		if (current_char == '(' || current_char == ')') {
			token = (char *)malloc(2 * sizeof(char));
			token[0] = current_char;
			token[1] = '\0';
			tokens[position++] = token;
			i++;
			continue;
		}

		// Gérer les opérateurs logiques et de redirection
		if (current_char == '>' || current_char == '<' ||
			(current_char == '&' && input[i+1] == '&') ||
			(current_char == '|' && input[i+1] == '|')) {
			token = (char *)malloc(3 * sizeof(char));  // Allouer de la mémoire
			if (current_char == '&' || current_char == '|') {
				token[0] = current_char;
				token[1] = input[i+1];
				token[2] = '\0';
				i += 2;
			} else {
				token[0] = current_char;
				token[1] = '\0';
				i++;
			}
			tokens[position++] = token;
			continue;
		}

		// Gérer les guillemets
		if (current_char == '"' || current_char == '\'') {
			char quote_type = current_char;
			i++;
			int start = i;
			while (input[i] != quote_type && input[i] != '\0') {
				i++;
			}
			if (input[i] == quote_type) {
				int length = i - start;
				token = (char *)malloc((length + 1) * sizeof(char));
				strncpy(token, &input[start], length);
				token[length] = '\0';
				tokens[position++] = token;
				i++;  // Passer le guillemet de fermeture
			} else {
				fprintf(stderr, "Erreur : guillemet non fermé\n");
				exit(EXIT_FAILURE);
			}
			continue;
		}

		// Traiter les autres tokens (commandes, arguments)
		int start = i;
		while (input[i] != '\0' && !isspace(input[i]) && input[i] != '(' && input[i] != ')' &&
			   input[i] != '>' && input[i] != '<' && input[i] != '&' && input[i] != '|') {
			i++;
		}
		int length = i - start;
		token = (char *)malloc((length + 1) * sizeof(char));
		strncpy(token, &input[start], length);
		token[length] = '\0';
		tokens[position++] = token;
	}

	tokens[position] = NULL;  // Fin du tableau de tokens
	return tokens;
}

int main() {
	char input[] = "(ls -l /home/user && echo \"done\") > output.txt";

	// Tokenization de l'entrée
	char **tokens = tokenize(input);

	// Affichage des tokens
	for (int i = 0; tokens[i] != NULL; i++) {
		printf("Token %d: %s\n", i, tokens[i]);
	}

	// Libérer la mémoire allouée pour les tokens
	for (int i = 0; tokens[i] != NULL; i++) {
		free(tokens[i]);
	}
	free(tokens);

	return 0;
}
