#include "copy_list.h"


/***********************************/
/** COPIER LA LISTE DES FICHIERS **/
/***********************************/
void copier_liste(LISTE* liste)
{
	// faut utiliser le module synch pour ajouter les nouveaus fichiers
	while( liste->taille != 0)
	{
		system("cp liste->deb_liste ./nouveau_serveur");
		supprimer_element_liste(liste, 0);
	}
	
	return;
}



