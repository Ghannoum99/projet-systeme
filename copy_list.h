#ifdef COPY_LIST_H
#define COPY_LIST_H
#include <stdio.h>
#include <stdlib.h>
#include "sync_list.h"
#include <dirent.h>
#include <conio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>




// Copier une liste des fichiers dans un répertoire
void copier_liste(LISTE* liste);


#endif
