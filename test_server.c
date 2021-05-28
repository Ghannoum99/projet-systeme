#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <semaphore.h>
#include "test_server.h"

//devrait renvoyer R = running / S = sleeping
char tester_disponibilite_serveur(pid_t servATester) {
	FILE* fp;
	char* commande = (char*) malloc(sizeof(char) * TAILLE_TAMPON );
	char* chaineEtat = (char*) malloc(sizeof(char) * TAILLE_TAMPON );
	char etatPID;
	
	//chercher l'état dans les processus
	sprintf(commande, "cat /proc/%ld/status | grep \"State\"", (long) servATester);
	fp = popen(commande, "r");
	
	//obtenir la lettre indiquant l'état
	fgets(chaineEtat, TAILLE_TAMPON, fp);
	etatPID = chaineEtat[7];
	
	printf("etat du PID %ld %c\n", (long) servATester, etatPID);
	
	pclose(fp);
	free(chaineEtat);
	
	return etatPID;
}
