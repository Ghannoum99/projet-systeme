#include "sync_list.h"


LISTE creer_liste_vide()
{
	LISTE liste;
	
	liste.deb_liste = NULL;
	liste.taille = 0;
	 
	return liste;
}

LISTE ajouter_element_liste(LISTE liste, FICHIER fichier)
{
	ELEMENT* nouveau = malloc(sizeof(ELEMENT*));
	
	nouveau->fichier = fichier;
	nouveau->suivant = liste.deb_liste;
	
	liste.deb_liste = nouveau;
	liste.taille++;

	return liste;
}

// Il faudrait ajouter la suppression ensuite

int main (void)
{
	DATE date;
	
	// Créer une liste
	
	// Lancer un appel système "ls" et le stocker dans un fichier texte (ou un buffer ?)
	
	// A partir de ce fichier texte récupérer les infos (nom + date) dans la liste
	
	// Supprimer le fichier texte
	
	return 0;
}
