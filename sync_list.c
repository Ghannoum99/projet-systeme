#include "sync_list.h"

LISTE* creer_liste_vide()
{
	LISTE* liste = NULL;
	
	liste = (LISTE*) malloc(sizeof(LISTE));
	
	liste->deb_liste = NULL;
	liste->taille = 0;
	 
	return liste;
}

void ajouter_element_liste(LISTE* liste, FICHIER fichier)
{
	ELEMENT* nouveau = (ELEMENT*)malloc(sizeof(ELEMENT));
	
	nouveau->fichier = fichier;
	nouveau->suivant = liste->deb_liste;
	
	liste->deb_liste = nouveau;
	liste->taille++;

}
void ajouter_element_liste_milieu(LISTE* liste, FICHIER fichier, int position)
{
	ELEMENT* element = liste->deb_liste;
	int fin_liste = 0;
	LISTE* liste_tampon = creer_liste_vide();
	
	//regarder si la position correspond au début de la liste ou non
	if(position == 0)
	{
		ajouter_element_liste(liste, fichier);
	}
	else
	{
		//Rechercher l'élement avant la position voulue
		for(int i = 0; i < position - 1; i++)
		{
			
			if(element->suivant == NULL) //s'assurer que la position voulue n'est pas hors de la liste
			{
				fin_liste = 1;
				break;
			}
			else
			{
				element = element->suivant;
			}
		}
		
		if(fin_liste)
		{
			//arriver à la fin de la liste donc on écrit juste dans la liste tampon vide l'élement
			ajouter_element_liste(liste_tampon, fichier);
		}
		else
		{
			//ajouter l'élement au début de la liste tampon qui commence à l'élement de la position voulu
			liste_tampon->deb_liste = element->suivant;
			ajouter_element_liste(liste_tampon, fichier);
		}
		
		//ratacher les deux listes ensembles
		element->suivant = liste_tampon->deb_liste;
		liste->taille ++;
	}
	free(liste_tampon);
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

void ajouter_nouveau_element_liste (LISTE* liste, char* nomFichier, char* nomServeur)
{
	FICHIER fichier;
	INFOCHANGE infos;
	time_t temps;
	char commande[TAILLE_MAX] = "touch ./";
	
	strcat(commande, nomServeur);
	strcat(commande, "/");
	strcat(commande, nomFichier);
	system(commande);
	
	strcpy(fichier.nom,nomFichier);
	temps = time(NULL);
	fichier.date = *localtime(&temps);
	ajouter_element_liste(liste,fichier);	
	
	infos.fichier = fichier;
	
	if(!strcmp(nomServeur, "servProd"))
		infos.origine = prod;
	else
		infos.origine = back;
		
	write(vers_inte[1], &infos, sizeof(INFOCHANGE));

}

void modifier_element_liste (LISTE* liste, char* nomElement, char* nomServeur)
{		
	ELEMENT* curseur = liste->deb_liste;
	time_t temps;
	FICHIER fichier;
	INFOCHANGE infos;
	char commande[TAILLE_MAX] = "touch ./";
	
	strcat(commande, nomServeur);
	strcat(commande, "/");
	strcat(commande, nomElement);
	system(commande);
	
	while (strcmp(curseur->fichier.nom, nomElement))
	{
		curseur = curseur->suivant;
		if (curseur == NULL)
		{
			printf("Le fichier n'a pas été trouvé");
			return;
		}
	}
	temps = time(NULL);
	curseur->fichier.date = *localtime(&temps);
	
	fichier.date = *localtime(&temps);
	strcpy(fichier.nom,nomElement);
	
	infos.fichier = fichier;
	
	if(!strcmp(nomServeur, "servProd"))
		infos.origine = prod;
	else
		infos.origine = back;
			
	write(vers_inte[1], &infos, sizeof(INFOCHANGE));

}
void modifier_fichier_liste(LISTE* liste, FICHIER fichier)
{
	ELEMENT* element = liste->deb_liste;
	int pos = 0;
	
	//recherche du fichier dans la liste
	while(element != NULL && strcmp(fichier.nom, element->fichier.nom))
	{
		pos++;
		element = element->suivant;
	}
	
	if(element == NULL)// le fichier n'existe pas donc on l'ajoute
	{
		ajouter_element_liste(liste, fichier);
	}
	else  // fichier existant donc on le remplace
	{
		supprimer_element_liste(liste,pos);
		ajouter_element_liste_milieu(liste, fichier,pos);
	}
}

void afficher_liste(LISTE liste)
{
	ELEMENT* curseur = liste.deb_liste;
	
	printf("Début d'affichage\n");
	
	while (curseur != NULL)
	{
		printf("%s | %s",curseur->fichier.nom, asctime(&curseur->fichier.date));
		curseur = curseur->suivant;
	}
	
	printf("\n");
	
}
