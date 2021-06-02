#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <semaphore.h>

#include "sync_list.h"
#include "test_server.h"

serveur servIntegr, servProd, servBackup;

//main = servIntegr
//fils = servProd / servBackup
int main(int argc, char* argv[]) {
	initialiser_serveur(&servIntegr, "servIntegr");
	initialiser_serveur(&servProd, "servProd");
	initialiser_serveur(&servBackup, "servBackup");
	
					
	/*
	LISTE* liste = creer_liste_vide();
	FICHIER infos;
	
	 
	if( pipe(tube) == -1)
	{
		perror("Les tubes ont un problème...");
		exit(EXIT_FAILURE);
	}
	
	
	system("mkdir serveurProd");
	//system("mkdir serveurBack");
	

	system("touch ./serveurProd/fichier1.txt");
	system("touch ./serveurProd/fichier2.txt");
		
	system("ls ./serveurProd > out.txt");
	lire_fichier("out.txt",liste);
	system("rm out.txt");	
	
	afficher_liste(*liste);
	*/
	
	servProd.pidServ = fork();
	
	switch(servProd.pidServ) {
		case -1 : //échec du fork
			perror("Échec fork");
			exit(EXIT_FAILURE);
			break;
			
		case 0 : //code du serveur de production
			printf("[%s] PID <%d>, PPID <%d>\n", servProd.nomServ, getpid(), getppid());
			
			/*
			close(tube[0]);
			sleep(2);
			modifier_element_liste (liste, "fichier1.txt");
			sleep(3);
			ajouter_nouveau_element_liste(liste, "fichier3.txt");
			afficher_liste(*liste);
			
			close(tube[1]);
			*/
			
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
					break;
				
				default : //code du serveur d'intégration
					printf("[%s] PID <%d>, PPID <%d>\n", servIntegr.nomServ, getpid(), getppid());

					pthread_create(&(servBackup.threads[1]),NULL,deverrouiller_serveur,(void*) &servBackup);
					pthread_join(servIntegr.threads[1],NULL);

					pthread_create(&(servIntegr.threads[1]),NULL,verrouiller_serveur,(void*) &servIntegr);
					pthread_join(servBackup.threads[1],NULL);
					
					afficher_etat_serveur(servBackup);
					afficher_etat_serveur(servIntegr);
					
					/*					
					close(tube[1]);
					
					read(tube[0],&infos,sizeof(FICHIER));
					read(tube[0],&infos,sizeof(FICHIER));
										
					close(tube[0]);
					
					afficher_liste(*liste);
					
					system("rm -r serveurProd");
					
					
					while (liste->taille != 0)
						supprimer_element_liste(liste,0);
		
					free(liste);
					*/
					
					wait(&servProd.pidServ);
					wait(&servBackup.pidServ);
					
					break;
			}
			
			break;
	}
	
	printf("*** Fin du process <%d> ***\n",getpid());
	sleep(1);
	
	return EXIT_SUCCESS;	
}
