#include <fcntl.h>

#include "sync_list.h"
#include "test_server.h"
#include "copy_list.h"
#include "log_stats.h"

int nommer_fichier(char* nom, int numero, char* nomServ)
{
	char nombre[TAILLE_MAX];
	
	sprintf(nombre,"%d",numero);
	
	nom[0] = '\0';
	if(!strcmp(nomServ, "servProd"))
	{
		strcat(nom,"fichierP");
	}
	else
	{
		strcat(nom,"fichierB");
	}
	strcat(nom,nombre);
	strcat(nom,".txt");
	
	return numero+1;
}

int ajouter (int numero, char* nomFichier, LISTE* liste, char* nomServ)
{
	char log[TAILLE_MAX] = "Ajout de ";
	
	numero = nommer_fichier(nomFichier,numero, nomServ);
	strcat(log, nomFichier);
	
	ajouter_nouveau_element_liste(liste,nomFichier,nomServ);
	ecrire_log("sync_list",log);
	
	return numero;
}


void modifier (int numero, char* nomFichier, LISTE* liste, char* nomServ)
{
	char log[TAILLE_MAX] = "Modification de ";
	
	numero = nommer_fichier(nomFichier,numero, nomServ);
	strcat(log, nomFichier);
	
	modifier_element_liste (liste,nomFichier,nomServ);
	ecrire_log("sync_list",log);	
}

serveur servIntegr, servProd, servBackup;

//main = servIntegr
//fils = servProd / servBackup
int main(int argc, char* argv[]) {
	
	LISTE* liste = creer_liste_vide();
	FICHIER donnees;
	char nomFichier[TAILLE_MAX] = "";
	int numero = 1, nbRandom = 0, i = 0;
	
	initialiser_serveur(&servIntegr, "servIntegr");
	initialiser_serveur(&servProd, "servProd");
	initialiser_serveur(&servBackup, "servBackup");	
	
	srand(time(NULL));
	
	if( pipe(vers_inte) == -1 || pipe(inte_back) == -1 || pipe(inte_prod) == -1 || pipe(serv_hs) == -1)
	{
		perror("Les tubes ont un problème...");
		exit(EXIT_FAILURE);
	}	
	
	//rendre le read non bloquant
	fcntl(inte_back[0], F_SETFL, O_NONBLOCK);
	fcntl(inte_prod[0], F_SETFL, O_NONBLOCK);
	fcntl(serv_hs[0], F_SETFL, O_NONBLOCK);

	nbRandom = rand()% 6 + 5;
	
	servProd.pidServ = fork();
	
	switch(servProd.pidServ) {
		case -1 : //échec du fork
			perror("Échec fork");
			exit(EXIT_FAILURE);
			break;
			
		case 0 : //code du serveur de production
			printf("[%s] PID <%d>, PPID <%d>\n", servProd.nomServ, getpid(), getppid());
								
			int nbRandom2 = 0;
			
			close(vers_inte[0]);
			close(inte_prod[1]);
			close(serv_hs[0]);
			
			sleep(rand()%3+1);
			for (i=0; i<nbRandom; i++)
			{
				if(i == 2 && nbRandom%2 == 0) // Le serveur se verrouille
				{
					pthread_create(&(servProd.threads[0]),NULL,verrouiller_serveur,(void*) &servProd);
					servProd.etatServ = pasDispo;
					write(serv_hs[1],&servProd.etatServ,sizeof(etat));
					pthread_create(&(servProd.threads[1]), NULL, deverrouiller_serveur, (void*) &servProd);
					pthread_join(servProd.threads[0],NULL);
					servProd.etatServ = dispo;
					write(serv_hs[1],&servProd.etatServ,sizeof(etat));
				}
				
				if(read(inte_prod[0],&donnees,sizeof(FICHIER)) == -1) //Si pas de fichier dans le tube  
				{
					nbRandom2 = rand()%2;
					
					if(nbRandom2 == 0 || numero == 1)
						numero = ajouter(numero,nomFichier,liste,"servProd");
					else
						modifier(rand()%(numero-1)+1,nomFichier,liste,"servProd");
						
					sleep(rand()%3+1);
				}
				else //fichier dans le tube
				{
					if(strcmp(donnees.nom,"end"))
						modifier_fichier_liste(liste, donnees);
					else
						break;
						
					sleep(rand()%3+1);
				}
			}
			
			if(strcmp(donnees.nom,"end"))
			{
				while(1)
				{
					if(read(inte_prod[0],&donnees,sizeof(FICHIER)) != -1) //On récupère les derniers fichiers
					{
						if (strcmp(donnees.nom,"end"))
							modifier_fichier_liste(liste, donnees);
						else
							break;
					}
				}
			}
			
			printf("Production\n");
			afficher_liste(*liste);
			
						
			close(vers_inte[1]);
			close(inte_prod[0]);
			close(serv_hs[1]);
			
			break;
			
		default :
			servBackup.pidServ = fork();
			
			switch(servBackup.pidServ) 
			{
				case -1 : //échec du fork
					perror("Échec fork");
					exit(EXIT_FAILURE);
					break;
			
				case 0 : //code du serveur de backup			
					printf("[%s] PID <%d>, PPID <%d>\n", servBackup.nomServ, getpid(), getppid());
		
					close(inte_back[1]);
					close(vers_inte[0]);
					
					sleep(rand()%3+1);
					for (i=0; i<nbRandom; i++)
					{
						if(read(inte_back[0],&donnees,sizeof(FICHIER)) == -1)//Si pas de fichier dans le tube  
						{
							nbRandom2 = rand()%2; nbRandom2 = rand()%2; // On double le rand pour pas avoir la même chose que dans le serveur de Production
							
							if(nbRandom2 == 0 || numero == 1)
								numero = ajouter(numero,nomFichier,liste,"servBackup");
							else
								modifier(rand()%(numero-1)+1,nomFichier,liste,"servBackup");
								
							sleep(rand()%3+1);
						}
						
						else //Si fichier dans le tube
						{
							modifier_fichier_liste(liste, donnees);
							sleep(rand()%3+1);
						}
				
					}
					
					while(1)
					{
						if(read(inte_back[0],&donnees,sizeof(FICHIER)) != -1) //On récupère les derniers fichiers
						{
							if (strcmp(donnees.nom,"end"))
								modifier_fichier_liste(liste, donnees);
							else
								break;
						}
					}			
					
					printf("Backup\n");
					afficher_liste(*liste);
					
					close(inte_back[0]);
					close(vers_inte[1]);
															
					break;
				
				default : //code du serveur d'intégration
					printf("[%s] PID <%d>, PPID <%d>\n", servIntegr.nomServ, getpid(), getppid());
					
					close(vers_inte[1]);
					close(inte_back[0]);
					close(inte_prod[0]);
					close(serv_hs[1]);
					
					LISTE* liste_changementB = creer_liste_vide();
					LISTE* liste_changementP = creer_liste_vide();

					INFOCHANGE infos;
					etat estDispoProd = dispo, tampon;
					ELEMENT* curseur;
					
					for(i = 0; i < nbRandom; i++)
					{
						read(vers_inte[0],&infos,sizeof(INFOCHANGE));

						modifier_fichier_liste(liste, infos.fichier);
							
						if(infos.origine == prod)
							modifier_fichier_liste(liste_changementP, infos.fichier);
						else
							modifier_fichier_liste(liste_changementB, infos.fichier);
							

						statistiques_module("sync_list", FICHIER_RECU);
						
							
						if(read(serv_hs[0],&tampon,sizeof(etat)) != -1)
							estDispoProd = tampon;
																										
						if(estDispoProd != dispo)
							statistiques_module("copy_list", ERREUR_RECONTRE);
						else 
						{
							if(liste_changementP->taille != 0)
							{
								write(inte_back[1], &infos.fichier, sizeof(FICHIER));
								copier_liste(liste_changementP, "./servProd/");
							}	
							if(liste_changementB->taille != 0)
							{								
								curseur = liste_changementB->deb_liste;
								while (curseur != NULL)
								{
									infos.fichier = curseur->fichier;
									write(inte_prod[1], &infos.fichier, sizeof(FICHIER));
									curseur = curseur->suivant;
									if (curseur != NULL)
										sleep(2);
								}
								copier_liste(liste_changementB, "./servBackup/");
							}
						}
					}

					infos.fichier.nom[0] = '\0';
					strcat(infos.fichier.nom,"end");
					write(inte_back[1],&infos.fichier, sizeof(FICHIER));
					write(inte_prod[1],&infos.fichier, sizeof(FICHIER));
					
					wait(&servProd.pidServ);
					wait(&servBackup.pidServ);
								
					close(vers_inte[0]);
					close(inte_back[1]);
					close(inte_prod[1]);
					close(serv_hs[0]);
					
					printf("Intégration\n");
					afficher_liste(*liste);
							
					printf("\n--------------- Contenu du serveur de production -------------\n");	
					system("ls -l ./servProd");
					printf("\n----------------- Contenu du serveur de backup ---------------\n");	
					system("ls -l ./servBackup");
					
					system("rm -r servProd");
					system("rm -r servBackup");
					
					free(liste_changementP);
					free(liste_changementB);
									
					break;
			}
			
			break;
	}
	
	while (liste->taille != 0)
		supprimer_element_liste(liste,0);
	
	free(liste);

	printf("*** Fin du process <%d> ***\n",getpid());
	
	return EXIT_SUCCESS;	
}
