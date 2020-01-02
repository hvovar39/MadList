#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>

#include "projet2019.h"
#include "affichage.h"

/*==========================================
FONCTIONS D'AFFICHAGE
==========================================*/

//affiche le noeud pointer par current (first sir current==liste)
void print_node (void *liste, void *current) {
  if (current == liste) {
    void *mem = ((head*)liste)->memory;
    current = (align_data *)mem + ((head*)liste)->first;
  }
  void *previous = ((align_data*)current) + ((node*)current)->previous;
  void *next = ((align_data*)current) + ((node*)current)->next;
  void *data = ((node *)current)+1;

  printf("node : %p, data : %p, previous : %p, next : %p\n", current, data, previous, next);
}


//affiche tous les noeuds de la liste dans l'ordre de parcours
void print_node_liste (void *liste) {
  if( ((head*)liste)->first != 0) {

    void * current = (align_data *)((head *)liste)->memory + ((head *)liste)->first;
    print_node (liste, current);
  
    while ( ((node*)current)->next ) {
      current = (align_data *)((node *)current) + ((node *)current)->next;
      print_node (liste, current);
    }
  }
    
}


void affichage_erreur (int i) {
  assert (i>0);
  switch (i) {
    
  case 1 :
    printf ("Erreur dans LD_CREATE\n");
    break;

  case 2 :
    printf ("Erreur dans LD_FIRST\n");
    break;
  }

}

int affichage_noeuds_int (void *liste) {
  int i = 1;
  void *current = ld_first (liste);
  if (current == NULL)
      return 2;
  void *data = malloc (sizeof(int));

  while ( ((node *)current)->next ) {
    ld_get (liste, current, sizeof(int), data);
    printf ("noeud %d : %d\n", i++, *(int *)data);
    current = ld_next (liste, current);
    if (current == NULL)
      return 4;
  }

  ld_get (liste, current, sizeof(int), data);
  printf ("noeud %d : %d\n", i++, *(int *)data);

  free (data);
  return 0;
}
