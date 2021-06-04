#include "copy_list.h"
#include "log_stats.h"

/**********************************/
/** COPIER LA LISTE DES FICHIERS **/
/**********************************/	
void copier_liste(LISTE* liste, char* nomSrc)
{
	char commande[TAILLE_MAX] = "";
	char nomDest[TAILLE_MAX] = "";
	char* nomFichier;
	ELEMENT* element;
	FICHIER fichier;
	
	if (!strcmp(nomSrc, "./servProd/"))
		strcat(nomDest, " ./servBackup/");
	else
		strcat(nomDest, " ./servProd/");
		
	while(liste->taille != 0)
	{
		commande[strlen(commande) - 1] = '\0';
		strcpy(commande, "cp ");
		
		element = liste->deb_liste;
		fichier = element->fichier;
		nomFichier = fichier.nom;
		
		strcat(commande,nomSrc);
		strcat(commande, nomFichier);
		strcat(commande, nomDest);
		
		system(commande);
		
		supprimer_element_liste(liste,0);
	}
	
	statistiques_module("copy_list",FICHIER_RECU);

	
	return;	
}

