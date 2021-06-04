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
	char creerDossier[TAILLE_MAX] = "mkdir ";
	
	strcat(creerDossier,nomServ);
	
	serv->etatServ = dispo;
	pthread_mutex_lock(&(serv->mutexServ));
	//pthread_mutex_lock(&(serv->mutexUnlock[0]));
	//pthread_mutex_lock(&(serv->mutexUnlock[1]));
	strcpy(serv->nomServ, nomServ);
	serv->nomServ[strlen(serv->nomServ)] = '\0';
	
	if(strcmp(nomServ,"servIntegr"))
		system(creerDossier);
	
}

/*
void* fct_test(void* serv) {
	pthread_mutex_lock(  &( ((serveur*)serv)->mutexUnlock[0]) );
	printf("C'est chiant cette fonction\n");	
	pthread_mutex_unlock(  &( ((serveur*)serv)->mutexUnlock[1]) );
	printf("J'ai envie de dormir\n");
	
	printf("J'ai fini mes trucs donc je déverrouille %s\n", servProd.nomServ);
	
	return NULL;
}*/

void* verrouiller_serveur(void* serv) {	
	//~ if( ((serveur*) serv)->etatServ == dispo ) {
	
	((serveur*) serv)->etatServ = pasDispo;
	printf("Verrouillage %s\n",  ((serveur*) serv)->nomServ);

	
	//pthread_mutex_unlock(  &( ((serveur*)serv)->mutexUnlock[0]) );
	pthread_mutex_lock(  &( ((serveur*)serv)->mutexServ) );
	//~ }
	
	
	return NULL;
}

void* deverrouiller_serveur(void* serv) {
	
	sleep(5);
	//sleep(rand()%10+1);
	
	//pthread_mutex_lock(  &( ((serveur*)serv)->mutexUnlock[1]) );
	
	//~ if( ((serveur*) serv)->etatServ == pasDispo ) {
	pthread_mutex_unlock( &( ((serveur*)serv)->mutexServ) );
	printf("Déverrouillage %s\n", ((serveur*) serv)->nomServ);
	((serveur*) serv)->etatServ = dispo;
	
	//~ }
	
	return NULL;
}

void afficher_etat_serveur(serveur serv) {
	if (serv.etatServ == dispo) {
		printf("Infos : %s | PID %4ld | etatServ %d -> disponible\n", serv.nomServ, (long) serv.pidServ, serv.etatServ);
	}
	else {
		printf("Infos : %s | PID %4ld | etatServ %d -> non disponible\n", serv.nomServ, (long) serv.pidServ, serv.etatServ);
	}
}
