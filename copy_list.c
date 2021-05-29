#include "copy_list.h"


void copier_liste(LISTE* liste)
{
	char commande[TAILLE_MAX] = "";
	char* nomRep = " ./serveurProd";
	char* nomFichier;
	ELEMENT* element;
	FICHIER fichier;
	
	while(liste->taille != 0)
	{
		commande[strlen(commande) - 1] = '\0';
		strcpy(commande, "cp ");
		
		element = liste->deb_liste;
		fichier = element->fichier;
		nomFichier = fichier.nom;
		
		strcat(commande,"./nov_serveur/");
		strcat(commande, nomFichier);
		strcat(commande, nomRep);
		
		system(commande);
		
		//printf("%s\n", commande);
		supprimer_element_liste(liste,0);
	}
	
	
	return;	
}



int main (void)
{

	LISTE* liste = creer_liste_vide();

	
	//system("mkdir serveurProd");
		
	system("ls ./nov_serveur > out.txt");
	lire_fichier("out.txt",liste);
	system("rm out.txt");
	
	afficher_liste(*liste);
	
	copier_liste(liste);
	
	
	
	return 0;
}
