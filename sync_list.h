#ifndef SYNC_LIST_H
#define SYNC_LIST_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TAILLE_MAX 100

int vers_inte[2];
int inte_prod[2];
int inte_back[2];

typedef struct tm DATE;
typedef enum {prod, back} serv;

// Structure d'un élément d'une liste de fichiers :

typedef struct FICHIER {
	char nom[TAILLE_MAX];
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

// Structure d'information :

typedef struct{
	FICHIER fichier;
	serv origine;
}INFOCHANGE;

// Prototypes de fonctions :

LISTE* creer_liste_vide();

void ajouter_element_liste(LISTE* liste, FICHIER fichier);
void ajouter_element_liste_milieu(LISTE* liste, FICHIER fichier, int position);
void supprimer_element_liste(LISTE* liste, int position);
void modifier_fichier_liste(LISTE* liste, FICHIER fichier);

void modifier_element_liste (LISTE* liste, char* nomElement, char* nomServeur);
void ajouter_nouveau_element_liste (LISTE* liste, char* nomFichier, char* nomServ);

void afficher_liste(LISTE liste);

#endif
