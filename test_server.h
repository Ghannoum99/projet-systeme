#ifndef TEST_SERVER_H
#define TEST_SERVER_H

#define P(s) sem_wait(s)
#define V(s) sem_post(s)

#define TAILLE_TAMPON 100
#define LONGUEUR_NOM_SERV 20

typedef enum {pasDispo, dispo} etat;

typedef struct {
	pid_t pidServ;
	pthread_mutex_t mutexServ;
	pthread_t threads[2]; //threads[0]=principal threads[1]=changer la disponibilité
	etat etatServ;
	char* nomServ;
} serveur;

char tester_disponibilite_serveur(pid_t servATester);

void initialiser_serveur(serveur* serv, char* nomServ);
void* verrouiller_serveur(void* serv);
void* deverrouiller_serveur(void* serv);
void afficher_etat_serveur(serveur serv);

#endif
