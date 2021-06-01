#ifndef SYNC_LIST_H
#define SYNC_LIST_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


#define TAILLE_MAX 100

int tube[2];

typedef struct tm DATE;

// Structure d'un élément d'une liste de fichiers :

typedef struct FICHIER {
	char* nom;
	DATE date;
} FICHIER;

typedef struct ELEMENT ELEMENT;
struct ELEMENT {
	FICHIER fichier;
	ELEMENT* suivant;
};

// Structure de liste :

typedef struct LISTE {
	ELEMENT* deb_liste;
	int taille;
} LISTE;

// Prototypes de fonctions :

LISTE* creer_liste_vide();
void ajouter_element_liste(LISTE* liste, FICHIER fichier);
void ajouter_element_liste_milieu(LISTE* liste, FICHIER fichier, int position);
void modifier_element_liste (LISTE* liste, char* nomElement);

void ajouter_nouveau_element_liste (LISTE*, char* nomFichier);
void supprimer_element_liste(LISTE* liste, int position);
LISTE faire_changement_liste(LISTE liste, FICHIER fichier);

void lire_fichier (char* nomFichier, LISTE* liste);
void afficher_liste(LISTE liste);

#endif
