#include "log_stats.h"

/*int main(){
	
	statistiques_module("module_1",FICHIER_RECU);
	
	return 0;
}*/

void ecrire_log(char* module, char* contenu){
	// Paramètres : 
	// Module : correspond au module qui souhaite écrire un log
	// Contenu : le contenu du log
	
	FILE* fichier = fopen("log.txt", "a");
    time_t now;

    time(&now);

	//strlen permet de vérifier la taille du contenu.
	//Un char ne valant que 1 octet
	if(strlen(contenu) > 128){ 
		// Ecriture du log échouée
		printf("-----\nEspace du contenu supérieur à 128 octects\n-----\n"); 
	}
	
	else{
		// Ecriture du log réussis
		printf("-----\nEcriture du log réussie.\n-----\n");
		fprintf(fichier,"%s | [%s] %s\n",contenu, module, ctime(&now));
	}
	
	fclose(fichier);
	
	return ;
}

void statistiques_module(char* module, int stat){
	char nom_fichier[12];
	// nombre de Fichier Recus
	char* nb_fr = NULL;
	// nombre d'Erreurs Rencontrées
	char* nb_er = NULL;
	int i = 0;
	time_t now;
	
	time(&now);
	
	nb_fr = (char*)malloc(sizeof(char));
	nb_er = (char*)malloc(sizeof(char));
	
	// Initialisation
	nom_fichier[0] = '\0';
	nb_fr[0] = '\0';
	nb_er[0] = '\0';
	
	// Permet d'obtenir le nom du fichier sur lequel écrire
	strcat(nom_fichier,module);
	strcat(nom_fichier,".txt");

	FILE* fichier = fopen(nom_fichier,"r+");
	rewind(fichier);

	// On récupère toutes le données de FR et ER
	for(i = 0; i < TMAX ; i++){
		nb_fr[i] = fgetc(fichier);
		if( nb_fr[i] == ',' ) {
			nb_fr[i] = '\0';
			i=TMAX;		
		}
	}
	
	for(i = 0; i < TMAX ; i++){
		nb_er[i] = fgetc(fichier);
		if( nb_er[i] == ',' ) {
			nb_er[i] = '\0';
			i=TMAX;		
		}
	}
	
	rewind(fichier);
	
	//Selon la stat passées en paramètre, on modifie l'information correspondante
	switch(stat){
		case FICHIER_RECU:
			ecrire_log(module, "Fichier reçu.");
			fprintf(fichier,"%d,%s,%s\nNombre de fichiers reçus,\nNombre d'erreurs recontrées,\nDate de la derniere modification",atoi(nb_fr)+1,nb_er,ctime(&now));
			break;
			
		case ERREUR_RECONTRE:
			ecrire_log(module, "Erreur recontrée.");
			fprintf(fichier,"%s,%d,%s\nNombre de fichiers reçus,\nNombre d'erreurs recontrées,\nDate de la derniere modification",nb_fr,atoi(nb_er)+1,ctime(&now));
			break;
			
		default:
			// Si la personne a fait une erreur au moment de l'appel de la fonction, un log d'erreur est écrit
			ecrire_log(module, "Error in -stat- entry!");
			break;
		
	}
	
	free(nb_fr);
	free(nb_er);
	
	fclose(fichier);
	
	return ;
}

