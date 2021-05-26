#ifndef SYNC_LIST_H
#define SYNC_LIST_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>


typedef struct tm* DATE;

	
// Structure d'un élément d'une liste de fichiers :

typedef struct FICHIER {
	char* nom;
	DATE date;
} FICHIER;

typedef struct ELEMENT ELEMENT;
struct ELEMENT {
	FICHIER* fichier;
	ELEMENT* suivant;
};

// Structure de liste :

typedef struct LISTE {
	ELEMENT* deb_liste;
	int taille;
} LISTE;

// Prototypes de fonctions :

void ajouter_element_liste(LISTE* liste, FICHIER* fichier);
LISTE* creer_liste_vide();

FICHIER* creer_fichier(char* nom);// a rajouter mais pas besoin pour test, DATE date)
void supprimer_element_liste(LISTE* liste, int position);
LISTE faire_changement_liste(LISTE liste, FICHIER fichier);

#endif
