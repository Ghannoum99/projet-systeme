#include "sync_list.h"


LISTE* creer_liste_vide()
{
	LISTE* liste = NULL;
	
	liste = (LISTE*) malloc(sizeof(LISTE));
	
	liste->deb_liste = NULL;
	liste->taille = 0;
	 
	return liste;
}

void ajouter_element_liste(LISTE* liste, FICHIER* fichier)
{
	ELEMENT* nouveau = (ELEMENT*)malloc(sizeof(ELEMENT));
	
	nouveau->fichier = fichier;
	nouveau->suivant = liste->deb_liste;
	
	liste->deb_liste = nouveau;
	liste->taille++;

}

void supprimer_element_liste(LISTE* liste, int position)
{
	/*premier de la liste est à la position 0*/
	
	ELEMENT* element = liste->deb_liste;
	ELEMENT* element_supprimer = liste->deb_liste;
	
	//ne pas essayer de supprimer un élement à une position impossible à atteindre
	if(liste->taille - 1 >= position && position >=0)
	{
		if(position == 0)
		{
			liste->deb_liste = element->suivant;
		}
		else
		{
			//aller jusqu'a la position avant l'element à supprimer
			for(int i = 0; i < position - 1 ; i ++)
			{
				element = element->suivant;
			}
			
			//enlever l'element de la liste 
			element_supprimer = element->suivant;
			element->suivant = element_supprimer->suivant;
		}
		free(element_supprimer);
		liste->taille--;
	}
}

// Il faudrait ajouter la suppression ensuite
FICHIER* creer_fichier(char* nom)// a rajouter mais pas besoin pour test, DATE date)
{
	FICHIER* fichier = NULL;
	fichier = (FICHIER*) malloc(sizeof(FICHIER));
	fichier->nom = nom;
	
	//element->date = date;
	
	return fichier;
}

/*int main (void)
{
	DATE date;
	
	// Créer une liste
	
	// Lancer un appel système "ls" et le stocker dans un fichier texte (ou un buffer ?)
	
	// A partir de ce fichier texte récupérer les infos (nom + date) dans la liste
	
	// Supprimer le fichier texte
	
	return 0;
}*/
