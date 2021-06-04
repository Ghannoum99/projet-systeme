#ifndef LOG_STATS_H
#define LOG_STATS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define TMAX 10000
#define FICHIER_RECU 1
#define ERREUR_RECONTRE 2

void ecrire_log(char* module, char* contenu);
void statistiques_module(char* module, int stat);

#endif
