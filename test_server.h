#ifndef TEST_SERVER_H
#define TEST_SERVER_H

typedef enum {False, True} bool;

#define P(s) sem_wait(s)
#define V(s) sem_post(s)

int tester_disponibilite_serveur(pid_t servATester);

#endif
