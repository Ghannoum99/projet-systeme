#include "copy_list.h"
#include "log_stats.h"


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
		
		//printf("%s\n", commande);
		supprimer_element_liste(liste,0);
	}
	
	statistiques_module("copy_list",FICHIER_RECU);

	
	return;	
}



/*int main (void)
{

	LISTE* liste = creer_liste_vide();

	
	system("mkdir serveurProd");
	system("mkdir serveurBack");
	

	system("touch ./serveurProd/fichier1.txt");
	system("touch ./serveurProd/fichier2.txt");
		
	system("ls ./serveurProd > out.txt");
	lire_fichier("out.txt",liste);
	system("rm out.txt");	
		
	afficher_liste(*liste);
	
	copier_liste(liste);
	
	afficher_liste(*liste);
	
	system("ls -l ./serveurProd");
	system("ls -l ./serveurBack");
	
	system("rm -r serveurProd");
	system("rm -r serveurBack");
		
	
	return 0;
}*/
