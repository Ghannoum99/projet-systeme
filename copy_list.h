#ifndef COPY_LIST_H
#define COPY_LIST_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "sync_list.h"
#include "test_server.h"

// Copier une liste des fichiers dans un répertoire
void copier_liste(LISTE* liste, char* NomSrc);


#endif
