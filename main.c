#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <semaphore.h>
#include "test_server.h"
#include "sync_list.h"

sem_t sem;

char estDispoProd = '\0', estDispoBackup = '\0';

//main = servIntegr
//fils = servProd / servBackup
int main(int argc, char* argv[]) {
	pid_t servProd, servBackup;
	
	/*
	 * Code du main de "sync_list.c"
	 */
	/*
	LISTE* liste = creer_liste_vide();
	
	creer_liste_vide(liste);
		
	system("ls > out.txt");
	lire_fichier("out.txt",liste);
	system("rm out.txt");	
	
	afficher_liste(*liste);
	
	while (liste->taille != 0)
		supprimer_element_liste(liste,0);
		
	free(liste);
	*/
	/*
	 * Fin du code du main de "sync_list.c"
	 */	
	
	servProd = fork();
	
	switch(servProd) {
		case -1 : //échec du fork
			perror("Échec fork");
			exit(EXIT_FAILURE);
			break;
			
		case 0 : //code du serveur de production
			printf("[servProd] PID <%d>, PPID <%d>\n", getpid(), getppid());
			break;
			
		default :
			servBackup = fork();
			
			switch(servBackup) {
				case -1 : //échec du fork
					perror("Échec fork");
					exit(EXIT_FAILURE);
					break;
			
				case 0 : //code du serveur de backup
					printf("[servBackup] PID <%d>, PPID <%d>\n", getpid(), getppid());
					break;
				
				default : //code du serveur d'intégration
					printf("[servIntegr] PID <%d>, PPID <%d>\n", getpid(), getppid());
					estDispoBackup = tester_disponibilite_serveur(servBackup);
					//~ estDispoProd = tester_disponibilite_serveur(servProd);
					printf("[servIntegr] estDispoBackup %c estDispoProd %c\n", estDispoBackup, estDispoProd);
					
					wait(NULL);
					break;
			}
			
			break;
	}
	
	printf("\n*** Fin du process <%d> ***\n",getpid());
	sleep(1);
	
	return EXIT_SUCCESS;	
}
