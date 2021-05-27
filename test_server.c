#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <semaphore.h>
#include "test_server.h"

sem_t sem;

int tester_disponibilite_serveur(pid_t servATester) {
	//kill prend en paramètre 0 pour ne pas tuer le pid
	if ( kill(servATester,0) == 0) {
		printf("%ld est disponible\n", (long) servATester);
		return True;
	}
	else {
		printf("%ld n'est pas disponible\n", (long) servATester);
		return False;
	}
}

int estDispoProd = False;
int estDispoBackup = False;

//main = serveur d'intégration
//fils = serveur de production et backup
int main(int argc, char* argv[]) {
	pid_t servProd, servBackup;
	
	sem_init(&sem, 0, 3);
	
	servProd = fork();
	
	switch(servProd) {
		case -1 : //échec du fork
			perror("Échec fork");
			exit(EXIT_FAILURE);
			break;
			
		case 0 : //code du serveur de production
			P(&sem);
			printf("[servProd] PID <%d>, PPID <%d>\n", getpid(), getppid());
			V(&sem);
			break;
			
		default :
			servBackup = fork();
			
			switch(servBackup) {
				case -1 : //échec du fork
					perror("Échec fork");
					exit(EXIT_FAILURE);
					break;
			
				case 0 : //code du serveur de backup
					P(&sem);
					printf("[servBackup] PID <%d>, PPID <%d>\n", getpid(), getppid());
					V(&sem);
					break;
				
				default : //code du serveur d'intégration
					//Corriger l'ordre avec les P et V
					P(&sem);
					P(&sem);
					P(&sem);
					printf("[servIntegr] PID <%d>, PPID <%d>\n", getpid(), getppid());
					estDispoBackup = tester_disponibilite_serveur(servBackup);
					estDispoProd = tester_disponibilite_serveur(servProd);
					printf("[servIntegr] estDispoBackup %d estDispoProd %d\n", estDispoBackup, estDispoProd);
					
					wait(NULL);
					break;
			}
			
			break;
	}
	
	printf("\n*** Fin du process <%d> ***\n",getpid());
	sleep(1);
	return EXIT_SUCCESS;	
}
