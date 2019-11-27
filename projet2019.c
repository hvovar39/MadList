#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>

#include "projet2019.h"
 


/*==========================================
HEADERS DES FONCTIONS AUXILIAIRES
==========================================*/
static size_t nb_blocs (size_t);
static void *test_memory (void *, size_t);
  
/*==========================================
FONCTIONS DE GESTION DE LISTE
==========================================*/

//crée toute les structure pour la liste

void *ld_create (size_t nboctets){
  size_t len = nb_blocs (nboctets) * sizeof (align_data);
  void *memory = malloc (len);
  head *liste = malloc (sizeof (head));
  liste->memory = memory;
  liste->first = 0;
  liste->last = 0;
  liste->libre = 0;
  liste->len = len;
  
  return liste;
}

//retourne le pointeur vers le premier noeud de la liste, ou NULL si la liste est vide

void *ld_first(void *liste){
  if (((head*)liste)->first == 0)
    return NULL;
  else
    return ((align_data *)((head*)liste)->memory) + ((head*)liste)->first;
}

//retourne le pointeur vers le dernier noeud de la liste, ou NULL si la liste est vide

void *ld_last(void *liste){ 
  if (((head*)liste)->first == 0)
    return NULL;
  else
    return ((align_data *)((head*)liste)->memory) + ((head*)liste)->last;
}

//current est un pointeur qui pointe soit vers head(i.e.liste==current) soit vers un nœud. Dans le premier cas,ld_next retourne la valeur de ld_first(liste). Dans le deuxième cas elle retourne le pointeur vers le nœud qui suit current sur la liste. Si current pointe vers le dernier nœud sur la liste, la fonction retourne NULL. Si la liste est vide et liste==current, la fonction retourne NULL

void *ld_next(void *liste, void *current){
  if ( liste == current )
      return ld_first(liste);
  else {
    void *last = ((align_data *)((head*)liste)->memory) + ((head*)liste)->last;
    if (((head*)liste)->first == 0 || (node*)current == (node *)last)
      return NULL;
    else
      return ((align_data *)((head*)liste)->memory) + ((node*)current)->next;
  }
}

//??????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????
//current est un pointeur qui pointe soit vers head(i.e.liste==current) soit vers un nœud. Dans le premier cas,ld_previous retourne la valeur de ld_last(liste)??????? <- est ce une bonne idee ? ne devrions nous pas plutot retourner NULL dans ce cas la ? pas clair dans le projet
//??????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????

//Dans le deuxième cas elle retourne le pointeur vers le nœud qui precede current sur la liste. Si current pointe vers le premier nœud sur la liste, la fonction retourne NULL. Si la liste est vide et liste==current, la fonction retourne NULL

void *ld_previous(void *liste, void *current){
  if ( liste == current )
      return ld_last(liste);
  else {
    void *last = ((align_data *)((head*)liste)->memory) + ((head*)liste)->last;
    if (((head*)liste)->first == 0 || (node*)current == (node *)last)
      return NULL;
    else
      return ((align_data *)((head*)liste)->memory) + ((node*)current)->previous;
  }
}

//la fonction detruit la liste en libérant la mémoire de head et de memory

void ld_destroy(void *liste){
  free (((head *)liste)->memory);
  free (liste);
}

//current est un pointeur vers un nœud (donc différent de liste).ld_get copie len octets du champ data du nœud current vers l’adresse val. Si le nombre d’octets demandé est supérieur au nombre d’octets dans le champ data du nœud, on limite la copie aux octets qui sont dans data. La fonction retourne le nombre d’octets copiés.

size_t ld_get(void *liste, void *current, size_t len, void *val){
  assert (len <= sizeof (*val));
  node *data = ((node *)current)+1;
  size_t taille_data = sizeof ( *data );

  if (len > taille_data) {
    memmove (val, data, taille_data);
    return taille_data;
  }

  memmove (val, data, len);
  return len;
}

//insère un nouveau nœud comme le premier élément de la liste

void * ld_insert_first(void *liste, size_t len, void *p_data){
  ptrdiff_t next;
  ptrdiff_t previous;
  size_t total_len = len + sizeof(node);
  void * new_node;

  test_memory (liste, total_len); //on verifie que l'on est assez de memoire, et le cas échéant, on augmente la memoire
  
  if( ((head *)liste)->first ) { //liste contenant au moins 1 element
    ptrdiff_t addrs_free_mem =  ((head *)liste)->len - nb_blocs (ld_total_useful_memory (liste));
    new_node = ((head *)liste)->memory + addrs_free_mem;
    
    void *current_first = (align_data *)( ((head *)liste)->memory + ((head *)liste)->first);
    ((node *)current_first)->previous = ((align_data *)new_node) -  ((align_data *)current_first);
    
    next = ((align_data *)current_first) - ((align_data *)new_node);
    previous = 0;
    ((head *)liste)->first = ((align_data *)new_node) - ((align_data *)((head *)liste)->memory);
    
  }
  
  else { //liste vide
    next = 0;
    previous = 0;
    new_node = (align_data *)( ((head *)liste)->memory + 1 );
    ((head *)liste)->first = 1;
    ((head *)liste)->last = 1;
  }
  
  node noeud = { .next = next, .previous = previous, .len = nb_blocs (total_len) };
  memmove (new_node, &noeud, sizeof(node));
  memmove ((node *)new_node+1, p_data, len);

  return new_node;
}

//insère un nouveau nœud comme le dernier élément de la liste

void * ld_insert_last(void *liste, size_t len, void *p_data){
 return NULL;
}

//insère un nouveau nœud dans la liste. Les paramètres len et p_data sont les mêmes que dans la fonction ld_insert_first. n est un pointeur vers un nœud, ld_insert_before insère le nouveau nœud avant le nœud n.

void * ld_insert_before(void*liste, void*n, size_t len, void*p_data){
 return NULL;
}


void * ld_insert_after (void *liste, void *n, size_t len, void *p_data){
 return NULL;
}


//supprime le noeud pointé par n. La fonction retourne liste quand la suppression réussit et NULL sinon.

void * ld_delete_node(void*liste, void*n){
 return NULL;
}

//retourne le nombre d’octets libres dans la mémoire memory. Cela permettra de voir si la mémoire commence à être saturée

size_t  ld_total_free_memory (void *liste){
  size_t free_mem = ((head*)liste)->len;
  
  void *current = liste;
  while ( ld_next(liste, current) != NULL)
    free_mem -= ((node*)current)->len;
      
  return free_mem * sizeof (align_data);
}

//retourne la taille de la mémoire libre qui peut être encore utilisée pour créer de nouveauxnœuds (si on ne réutilise pas la mémoire libérée cela peut être beaucoup moins que ce queretourne la fonction précédente qui compte aussi la mémoire libre mais non utilisable).

size_t  ld_total_useful_memory (void*liste){
  void * last_node = (align_data *)((head *)liste)->memory + ((head *)liste)->last ;
  size_t size_last_node = ((node *)last_node)->len;
  size_t useful_mem = ((head *)liste)->len - ( ((head *)liste)->last + size_last_node);
  
  return useful_mem * sizeof (align_data);
}

//agrandit la mémoire de nb_octets. Comme pour la création de la liste, on arrondit nb_octets vers un multiple de sizeof(align_data). Il est impossible de diminuer la taille de mémoire. La fonction retourne NULL en cas deproblème et liste sinon.

void*ld_add_memory(void*liste, size_t nboctets){
  size_t taille = ((head *)liste)->len + nb_blocs (nboctets);
  void * temp = realloc ( ((head *)liste)->memory, taille * sizeof (align_data));

  if (temp != NULL){
    ((head *)liste)->memory = temp;
    ((head *)liste)->len = taille;
    return liste;
  }
 return NULL;
}

//compacte la liste en mettant les nœuds au début de la mémoire avec une seule tranche de blocs libres à la fin de la mémoire.(La fonction devra sans doute allouer une nouvelle mémoire (de la même taille que l’ancienne) pour y recopier tous les nœuds l’un après l’autre dans l’ordre de parcours sur la liste.) La fonction retourne NULL en cas de problème (échec de malloc), sinon elle retourne liste.

void*ld_compactify(void*liste){
  size_t size_of_node = nb_blocs (sizeof (node));
  void * new_liste = malloc (sizeof ( *((head*)liste)->memory) );
  if (new_liste == NULL)
    return NULL;

  void *current = (align_data *)((head *)liste)->memory + ((head *)liste)->first;

  while ( ((node *)current)->next ) {
    size_t taille_data = ((node *)current)->len - size_of_node;
    

  return NULL;
}


/*==========================================
FONCTIONS COMPLEMENTAIRES
==========================================*/

//retourne le nombre de bloc de taille sizeof(align_data) pour stocker o octets
static size_t nb_blocs (size_t o){
  size_t align_data_size = sizeof(align_data);
  size_t result = o/align_data_size;
  
  if (o%align_data_size)
    return result+1;

  return result;
}

//test si la mémoir peut contenir len octets de plus, sinon, rajoute 1000 octets
static void *test_memory (void *liste, size_t len) {
  void * result = NULL;
  if ( ld_total_useful_memory (liste) <= len)
     result = ld_add_memory (liste, 1000);
  return result;
}
