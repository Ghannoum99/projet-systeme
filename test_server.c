#include "test_server.h"

extern serveur servIntegr, servProd, servBackup;

/*******************************/
/** INITIALISER LES SERVEURS  **/
/*******************************/	
void initialiser_serveur(serveur* serv, char* nomServ) {
	char creerDossier[TAILLE_MAX] = "mkdir ";
	
	strcat(creerDossier,nomServ);
	
	serv->etatServ = dispo;
	pthread_mutex_lock(&(serv->mutexServ));
	strcpy(serv->nomServ, nomServ);
	serv->nomServ[strlen(serv->nomServ)] = '\0';
	
	if(strcmp(nomServ,"servIntegr"))
		system(creerDossier);
	
}

/****************************************************/
/** VERROUILLER UN SERVEUR (DEVIENT INUTILISABLE)  **/
/****************************************************/	
void* verrouiller_serveur(void* serv) {	
	
	printf("Verrouillage %s\n",  ((serveur*) serv)->nomServ);
	pthread_mutex_lock(  &( ((serveur*)serv)->mutexServ) );	
	
	return NULL;
}

/*******************************/
/** DEVERROUILLER UN SERVEUR  **/
/*******************************/	
void* deverrouiller_serveur(void* serv) {
	
	sleep(rand()%5+1);
	
	pthread_mutex_unlock( &( ((serveur*)serv)->mutexServ) );
	printf("Déverrouillage %s\n", ((serveur*) serv)->nomServ);
		
	return NULL;
}
