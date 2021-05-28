#ifndef TEST_SERVER_H
#define TEST_SERVER_H

#define P(s) sem_wait(s)
#define V(s) sem_post(s)

#define TAILLE_TAMPON 100

char tester_disponibilite_serveur(pid_t servATester);

#endif
