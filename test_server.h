#ifndef TEST_SERVER_H
#define TEST_SERVER_H

#define TAILLE_TAMPON 100
#define TAILLE_MAX 100
#define LONGUEUR_NOM_SERV 100
#define OUI 1
#define NON 0

int serv_hs[2];

typedef enum {pasDispo, dispo} etat;

typedef struct {
	pid_t pidServ;
	char nomServ[LONGUEUR_NOM_SERV];
	pthread_mutex_t mutexServ;
	//pthread_mutex_t mutexUnlock[2];
	pthread_t threads[2];
	etat etatServ;
} serveur;

char tester_disponibilite_serveur(pid_t servATester);

void initialiser_serveur(serveur* serv, char* nomServ);
void* verrouiller_serveur(void* serv);
void* deverrouiller_serveur(void* serv);
void afficher_etat_serveur(serveur serv);



void* fct_test(void* serv);

#endif
