//
// Created by clesucre on 7/10/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h> // Ajouter cette ligne pour wait()

void handle_signal(int sig) {
	if (sig == SIGUSR1) {
		printf("Received SIGUSR1 signal\n");
	}
}

int main() {
	pid_t pid;

	// Configurer le handler de signal pour le processus enfant
	struct sigaction sa;
	sa.sa_handler = handle_signal;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);

	pid = fork();
	if (pid == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (pid == 0) {  // Code pour le processus enfant
		if (sigaction(SIGUSR1, &sa, NULL) == -1) {
			perror("sigaction");
			exit(EXIT_FAILURE);
		}
		printf("Child process waiting for signal...\n");

		// Exécuter la commande `cat`
		execlp("cat", "cat", NULL);
		// Si execlp échoue
		perror("execlp");
		exit(EXIT_FAILURE);
	} else {  // Code pour le processus parent
		sleep(5); // Assurez-vous que l'enfant est prêt à recevoir le signal
		printf("Parent process sending signal to child...\n");
		kill(pid, SIGUSR1); // Envoyer le signal SIGUSR1 à l'enfant

		// Attendre que l'enfant traite le signal
		wait(NULL);
	}

	return 0;
}
