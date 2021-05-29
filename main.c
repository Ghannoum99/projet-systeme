#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <semaphore.h>
#include "test_server.h"
#include "sync_list.h"

serveur servIntegr, servProd, servBackup;

//main = servIntegr
//fils = servProd / servBackup
int main(int argc, char* argv[]) {
	initialiser_serveur(&servIntegr, "servIntegr");
	initialiser_serveur(&servProd, "servProd");
	initialiser_serveur(&servBackup, "servBackup");
	
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
	
	servProd.pidServ = fork();
	
	switch(servProd.pidServ) {
		case -1 : //échec du fork
			perror("Échec fork");
			exit(EXIT_FAILURE);
			break;
			
		case 0 : //code du serveur de production
			printf("[%s] PID <%d>, PPID <%d>\n", servProd.nomServ, getpid(), getppid());
			verrouiller_serveur(&servProd);
			
			break;
			
		default :
			servBackup.pidServ = fork();
			
			switch(servBackup.pidServ) {
				case -1 : //échec du fork
					perror("Échec fork");
					exit(EXIT_FAILURE);
					break;
			
				case 0 : //code du serveur de backup
					printf("[%s] PID <%d>, PPID <%d>\n", servBackup.nomServ, getpid(), getppid());
					verrouiller_serveur(&servBackup);
					break;
				
				default : //code du serveur d'intégration
					
					printf("[%s] PID <%d>, PPID <%d>\n", servIntegr.nomServ, getpid(), getppid());
					
					deverrouiller_serveur(&servBackup);
					afficher_etat_serveur(servProd);
					afficher_etat_serveur(servBackup);
					
					break;
			}
			
			break;
	}
	
	printf("*** Fin du process <%d> ***\n",getpid());
	sleep(1);
	
	return EXIT_SUCCESS;	
}
