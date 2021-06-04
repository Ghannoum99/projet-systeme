#ifndef TEST_SERVER_H
#define TEST_SERVER_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <pthread.h>
#include <stdlib.h>

#include "sync_list.h"

#define TAILLE_TAMPON 100
#define LONGUEUR_NOM_SERV 100

int serv_hs[2];

typedef enum {pasDispo, dispo} etat;

typedef struct {
	pid_t pidServ;
	char nomServ[LONGUEUR_NOM_SERV];
	pthread_mutex_t mutexServ;
	pthread_t threads[2];
	etat etatServ;
} serveur;

void initialiser_serveur(serveur* serv, char* nomServ);
void* verrouiller_serveur(void* serv);
void* deverrouiller_serveur(void* serv);

#endif
