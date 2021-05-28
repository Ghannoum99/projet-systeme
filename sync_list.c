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
		free(element_supprimer->fichier.nom);
		free(element_supprimer);
		liste->taille--;
	}
}

void ajouter_nouveau_element_liste (LISTE* liste, char* nomFichier)
{
	FICHIER infos;
	time_t temps;
	char commande[TAILLE_MAX] = "touch ./serveurProd/";
	
	strcat(commande, nomFichier);
	system(commande);
	
	infos.nom = malloc(strlen(nomFichier) * sizeof(char));
	strcpy(infos.nom,nomFichier);
	temps = time(NULL);
	infos.date = *localtime(&temps);
	ajouter_element_liste(liste,infos);	
	
	write(tube[1], &infos, sizeof(FICHIER));

}

void lire_fichier (char* nomFichier, LISTE* liste)
{
	FILE* fichier = NULL;
	char tampon[TAILLE_MAX] =  "";
	time_t temps;
	FICHIER infos;
			
	fichier = fopen(nomFichier, "r");
	
	while(fgets(tampon, TAILLE_MAX, fichier) != NULL)
	{
		tampon[strlen(tampon) - 1] = '\0';
		infos.nom = malloc(strlen(tampon) * sizeof(char));
		strcpy(infos.nom,tampon);

		temps = time(NULL);
		infos.date = *localtime(&temps);
		ajouter_element_liste(liste,infos);
	}
	
	fclose(fichier);
}

void modifier_element_liste (LISTE* liste, char* nomElement)
{	
	ELEMENT* curseur = liste->deb_liste;
	time_t temps;
	FICHIER infos;
	
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
	
	infos.date = *localtime(&temps);
	infos.nom = malloc(strlen(nomElement) * sizeof(char));
	strcpy(infos.nom,nomElement);

	write(tube[1], &infos, sizeof(FICHIER));

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

/*
int main (void)
{
	pid_t PIDProd, PIDBack;	
	LISTE* liste = creer_liste_vide();
	FICHIER test;
	
	if( pipe(tube) == -1)
	{
		perror("Les tubes ont un problème...");
		exit(EXIT_FAILURE);
	}
	
	system("mkdir serveurProd");
	//system("mkdir serveurBack");
	

	system("touch ./serveurProd/fichier1.txt");
	system("touch ./serveurProd/fichier2.txt");
		
	system("ls ./serveurProd > out.txt");
	lire_fichier("out.txt",liste);
	system("rm out.txt");	
	
	afficher_liste(*liste);
	
	PIDProd = fork();
	
	/*if (PIDProd != 0)
		PIDBack = fork();*/ // Pas bseoin pour le moment
		
	
	if (PIDProd == 0)
	{
		close(tube[0]);
		
		sleep(2);
		modifier_element_liste (liste, "fichier1.txt");
		sleep(3);
		ajouter_nouveau_element_liste(liste, "fichier3.txt");
		afficher_liste(*liste);
		
		close(tube[1]);
	}
	
	else
	{
		close(tube[1]);
		
		read(tube[0],&test,sizeof(FICHIER));
		read(tube[0],&test,sizeof(FICHIER));
		
		wait(NULL);
		
		close(tube[0]);
		
		afficher_liste(*liste);
		
		system("rm -r serveurProd");
	
		while (liste->taille != 0)
			supprimer_element_liste(liste,0);
	}
	
	return 0;
}
*/
