#ifndef TEST_SERVER_H
#define TEST_SERVER_H

#define P(s) sem_wait(s)
#define V(s) sem_post(s)

#define TAILLE_TAMPON 100
#define TAILLE_NOM_SERV 20

typedef enum {pasDispo, dispo} etat;

typedef struct {
	pid_t pidServ;
	pthread_mutex_t mutexServ;
	etat etatServ;
	char* nomServ;
} serveur;

char tester_disponibilite_serveur(pid_t servATester);

void initialiser_serveur(serveur* serv, char* nomServ);
void verrouiller_serveur(serveur* serv);
void deverrouiller_serveur(serveur* serv);
void afficher_etat_serveur(serveur serv);

#endif
