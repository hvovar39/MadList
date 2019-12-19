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
