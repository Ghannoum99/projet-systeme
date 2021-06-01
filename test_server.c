#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <semaphore.h>

#include "test_server.h"

extern serveur servIntegr, servProd, servBackup;

//pas sûr que cette fonction serve mais je laisse
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

void initialiser_serveur(serveur* serv, char* nomServ) {
	serv->etatServ = pasDispo;
	pthread_mutex_lock(&(serv->mutexServ));
	serv->nomServ = malloc(sizeof(char) * TAILLE_NOM_SERV);
	strcpy(serv->nomServ, nomServ);
}

void verrouiller_serveur(serveur* serv) {
	serv->etatServ = pasDispo;
	pthread_mutex_lock(&(serv->mutexServ));
}

void deverrouiller_serveur(serveur* serv) {
	pthread_mutex_unlock(&(serv->mutexServ));
	serv->etatServ = dispo;
}

void afficher_etat_serveur(serveur serv) {
	if (serv.etatServ == dispo) {
		printf("%s PID %ld etatServ %d -> disponible\n", serv.nomServ, (long) serv.pidServ, serv.etatServ);
	}
	else {
		printf("%s PID %ld etatServ %d -> non disponible\n", serv.nomServ, (long) serv.pidServ, serv.etatServ);
	}
}
