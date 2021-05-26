#ifndef SYNC_LIST_H
#define SYNC_LIST_H

#include <stdio.h>
#include <time.h>


typedef struct tm* DATE;

	
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

LISTE ajouter_element_liste(LISTE liste, FICHIER fichier);
LISTE creer_liste_vide();


#endif
