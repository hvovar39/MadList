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
static void *last_in_mem (void *);
  
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
  liste->len = nb_blocs (nboctets);
  liste->nbr_noeud = 0;
  
  return liste;
}

//retourne le pointeur vers le premier noeud de la liste, ou NULL si la liste est vide

void *ld_first(void *liste){
  if (((head*)liste)->first == 0)
    return NULL;
  else
    return (align_data *)((head*)liste)->memory + ((head*)liste)->first;
}

//retourne le pointeur vers le dernier noeud de la liste, ou NULL si la liste est vide

void *ld_last(void *liste){ 
  if (((head*)liste)->first == 0)
    return NULL;
  else
    return (align_data *)((head*)liste)->memory + ((head*)liste)->last;
}

//current est un pointeur qui pointe soit vers head(i.e.liste==current) soit vers un nœud. Dans le premier cas,ld_next retourne la valeur de ld_first(liste). Dans le deuxième cas elle retourne le pointeur vers le nœud qui suit current sur la liste. Si current pointe vers le dernier nœud sur la liste, la fonction retourne NULL. Si la liste est vide et liste==current, la fonction retourne NULL

void *ld_next(void *liste, void *current){
  if ( liste == current ){
    if ( ((head *)liste)->first == 0)
      return NULL;
    else
      return ld_first(liste);
    
  } else if ( ((node*)current)->next == 0 )
    return NULL;
  else
    return (align_data *)current + ((node *)current)->next; 
}

//current est un pointeur qui pointe soit vers head(i.e.liste==current) soit vers un nœud. Dans le premier cas,ld_previous retourne la valeur de ld_last(liste)??????? <- est ce une bonne idee ? ne devrions nous pas plutot retourner NULL dans ce cas la ? pas clair dans le projet. Dans le deuxième cas elle retourne le pointeur vers le nœud qui precede current sur la liste. Si current pointe vers le premier nœud sur la liste, la fonction retourne NULL. Si la liste est vide et liste==current, la fonction retourne NULL

void *ld_previous(void *liste, void *current){
  if ( liste == current )
      return ld_last(liste);
  else if ( ((head*)liste)->first == 0 || ((node*)current)->previous == 0 )
    return NULL;
  else
    return (align_data *)current + ((node *)current)->previous;
}

//la fonction detruit la liste en libérant la mémoire de head et de memory

void ld_destroy(void *liste){
  free (((head *)liste)->memory);
  free (liste);
}

//current est un pointeur vers un nœud (donc différent de liste).ld_get copie len octets du champ data du nœud current vers l’adresse val. Si le nombre d’octets demandé est supérieur au nombre d’octets dans le champ data du nœud, on limite la copie aux octets qui sont dans data. La fonction retourne le nombre d’octets copiés.

size_t ld_get(void *liste, void *current, size_t len, void *val){
  node *data = ((node *)current)+1;
  size_t taille_data = (((node *)current)->len * sizeof(align_data))  - sizeof(node);

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
    ptrdiff_t addrs_free_mem =  ((head *)liste)->len - nb_blocs (ld_total_useful_memory (liste)) + 1;
    new_node = (align_data *)((head *)liste)->memory + addrs_free_mem;
    
    void *current_first = (align_data *)((head *)liste)->memory + ((head *)liste)->first;
    ((node *)current_first)->previous = (align_data *)new_node - (align_data *)current_first;
    
    next = (align_data *)current_first - (align_data *)new_node;
    previous = 0;
    ((head *)liste)->first = (align_data *)new_node - (align_data *)((head *)liste)->memory;
  }
  
  else { //liste vide
    next = 0;
    previous = 0;
    new_node = (align_data *)((head *)liste)->memory + 1;
    ((head *)liste)->first = 1;
    ((head *)liste)->last = 1;
  }
  
  node noeud = { .next = next, .previous = previous, .len = nb_blocs (total_len) };
  memmove (new_node, &noeud, sizeof(node));
  memmove ((node *)new_node+1, p_data, len);
  ((head *)liste)->nbr_noeud ++;

  return new_node;
}

//insère un nouveau nœud comme le dernier élément de la liste

void * ld_insert_last(void *liste, size_t len, void *p_data){
  ptrdiff_t next;
  ptrdiff_t previous;
  size_t total_len = len + sizeof(node);
  void * new_node;

  test_memory (liste, total_len); //on verifie que l'on est assez de memoire, et le cas échéant, on augmente la memoire
  
  if( ((head *)liste)->first ) {//liste contenant au moins 1 element
    ptrdiff_t addrs_free_mem =  ((head *)liste)->len - nb_blocs (ld_total_useful_memory (liste)) +1;
    new_node = (align_data*)((head *)liste)->memory + addrs_free_mem; //on se place à la fin de la memoire utilisé
    
    void *current_last = (align_data *)((head *)liste)->memory + ((head *)liste)->last; //on se place au niveau du dernier actuel
    ((node *)current_last)->next = (align_data *)new_node -  (align_data *)current_last; //pour placer notre nouveau noeud derriere
    
    next =0; //le dernier n'a pas de suivant
    previous = ((align_data *)current_last) - ((align_data *)new_node) ; 
    ((head *)liste)->last = ((align_data *)new_node) - ((align_data *)((head *)liste)->memory); //on met notre nouveau noeud en dernier
    
  }
  
  else { //liste vide donc pas de prochain ou de précédant le dernier de la liste et aussi le premier
    next = 0;
    previous = 0;
    new_node = (align_data *)(((head *)liste)->memory) + 1;
    ((head *)liste)->first = 1;
    ((head *)liste)->last = 1;
  }
  
  node noeud = { .next = next, .previous = previous, .len = nb_blocs (total_len) };
  memmove (new_node, &noeud, sizeof(node));
  memmove ((node *)new_node+1, p_data, len);
  ((head *)liste)->nbr_noeud ++;

  return new_node;
}

//insère un nouveau nœud dans la liste. Les paramètres len et p_data sont les mêmes que dans la fonction ld_insert_first. n est un pointeur vers un nœud, ld_insert_before insère le nouveau nœud avant le nœud n.

void * ld_insert_before(void*liste, void*n, size_t len, void*p_data){
  ptrdiff_t next;
  ptrdiff_t previous;
  size_t total_len = len + sizeof(node);
  void * new_node;

  test_memory (liste, total_len); //on verifie que l'on est assez de memoire, et le cas échéant, on augmente la memoire

  if( ((head *)liste)->first == (align_data *)n - (align_data *)((head *)liste)->memory )
    ld_insert_first (liste, len, p_data);
      
  assert( ((head *)liste)->first ); //liste contenant au moins 2 element & n n'est pas le premier
  
  ptrdiff_t addrs_free_mem =  ((head *)liste)->len - nb_blocs (ld_total_useful_memory (liste)) + 1;
  new_node = (align_data *)((head *)liste)->memory + addrs_free_mem; //on se place à la fin de la memoire utilisé
  
  void *p = (align_data *)n + ((node *)n)->previous; //on se place au niveau du precedant actuel de n
  ((node *)p)->next = (align_data *)new_node - (align_data *)p ; //le nouveau noeud
  ((node *)n)->previous = (align_data *)new_node - (align_data *)n ; //le nouveau noeud
  
  next = (align_data *)n - (align_data *)new_node ; //le noeud n
  previous = (align_data *)p - (align_data *)new_node ; //le noeud p
  
  node noeud = { .next = next, .previous = previous, .len = nb_blocs (total_len) };
  memmove (new_node, &noeud, sizeof(node));
  memmove ((node *)new_node+1, p_data, len);
  ((head *)liste)->nbr_noeud ++;

  return new_node;
}


void * ld_insert_after (void *liste, void *n, size_t len, void *p_data){
  ptrdiff_t next;
  ptrdiff_t previous;
  size_t total_len = len + sizeof(node);
  void * new_node;

  if( ((head *)liste)->last == (align_data *)n - (align_data *)((head *)liste)->memory )
    return ld_insert_last(liste, len, p_data);

  test_memory (liste, total_len); //on verifie que l'on est assez de memoire, et le cas échéant, on augmente la memoire
   
  assert( ((head *)liste)->first ); //liste contenant au moins 2 element & n n'est pas le dernier
  
  ptrdiff_t addrs_free_mem =  ((head *)liste)->len - nb_blocs (ld_total_useful_memory (liste));
  new_node = (align_data *)((head *)liste)->memory + addrs_free_mem; //on se place à la fin de la memoire utilisé
  
  void *p = (align_data *)n + ((node *)n)->next; //on se place au niveau du suivant actuel de n
  ((node *)p)->previous = (align_data *)new_node - (align_data *)p ; //new_node devient le precedent de p
  ((node *)n)->next = (align_data *)new_node - (align_data *)n ; //new_node devient le suivant de n
  
  previous = (align_data *)n - (align_data *)new_node ; //le noeud n devient le precedent de new_node
  next = (align_data *)p - (align_data *)new_node ; //le noeud p devient le suivant de new_node
  
  node noeud = { .next = next, .previous = previous, .len = nb_blocs (total_len) };
  memmove (new_node, &noeud, sizeof(node));
  memmove ((node *)new_node+1, p_data, len);
  ((head *)liste)->nbr_noeud ++;

  return new_node;
}


//supprime le noeud pointé par n. La fonction retourne liste quand la suppression réussit et NULL sinon. On suppose que le noeud n est forcement dans la liste et donc que la liste est non vide.

void * ld_delete_node(void*liste, void*n){
  void * next;
  void * previous;

  if(((node *)n)->previous == 0 && ((node *)n)->next == 0){
    ((head * )liste)->first=0;
    ((head*)liste)->last=0;
  }
  
  else if (((node *)n)->previous == 0){
    next =ld_next(liste,n);
    ((node * )next)->previous = 0;
    ((head * )liste)->first=next - ((head * )liste)->memory;
  }
  
  else if (((node *)n)->next == 0){
    previous =ld_previous(liste,n);
    ((node * )previous)->next = 0;
    ((head*)liste)->last=((align_data *)previous) - ((align_data *)((head * )liste)->memory);
  }
  
  else {
    next =ld_next(liste,n);
    previous =ld_previous(liste,n);
    ((node * )next)->previous =(align_data*)previous-(align_data*)next ;
    ((node * )previous)->next = (align_data*)next-(align_data*)previous;
  }
  ((head *)liste)->nbr_noeud --;

  return liste;
}

//retourne le nombre d’octets libres dans la mémoire memory. Cela permettra de voir si la mémoire commence à être saturée

size_t  ld_total_free_memory (void *liste){
  size_t free_mem = ((head*)liste)->len;
  
  void *current = ld_first (liste);
  free_mem -= ((node *)current)->len; 

  while ( ((node *)current)->next != 0){
    current = ld_next (liste, current);
    free_mem -= ((node*)current)->len;
  }
      
  return free_mem * sizeof (align_data);
}

//retourne la taille de la mémoire libre qui peut être encore utilisée pour créer de nouveauxnœuds (si on ne réutilise pas la mémoire libérée cela peut être beaucoup moins que ce queretourne la fonction précédente qui compte aussi la mémoire libre mais non utilisable).

size_t  ld_total_useful_memory (void*liste){
  size_t result = ((head*)liste)->len * sizeof(align_data);
  
  if( ((head*)liste)->first) {
    void * last_node = last_in_mem (liste);
    result -= (char *)((align_data *)last_node + ((node*)last_node)->len) - (char *)((head*)liste)->memory;
  }

  return result;
}

//agrandit la mémoire de nb_octets. Comme pour la création de la liste, on arrondit nb_octets vers un multiple de sizeof(align_data). Il est impossible de diminuer la taille de mémoire. La fonction retourne NULL en cas deproblème et liste sinon.

void *ld_add_memory (void *liste, size_t nboctets){
  size_t taille = nb_blocs (((head *)liste)->len + nboctets );
  void * temp = realloc ( ((head *)liste)->memory, taille * sizeof (align_data));

  printf("LISTE LEN : %lu\n", ((head *)liste)->len);  
  
  if (temp != NULL){
    ((head *)liste)->memory = temp;
    ((head *)liste)->len = taille;
    printf("LISTE LEN : %lu\n", ((head *)liste)->len);
    return liste;
  }
    
  return NULL;
}

//compacte la liste en mettant les nœuds au début de la mémoire avec une seule tranche de blocs libres à la fin de la mémoire.(La fonction devra sans doute allouer une nouvelle mémoire (de la même taille que l’ancienne) pour y recopier tous les nœuds l’un après l’autre dans l’ordre de parcours sur la liste.) La fonction retourne NULL en cas de problème (échec de malloc), sinon elle retourne liste.

void *ld_compactify(void*liste){
  size_t size_of_liste = ((head*)liste)->len;
  void * new_liste = ld_create ( size_of_liste );
  if (new_liste == NULL)
    return NULL;
  size_t taille_data;
  
  if (new_liste == NULL)
    return NULL;
  
  void *current = (align_data *)((head *)liste)->memory + ((head *)liste)->first;
  do{
    taille_data = (((node *)current)->len * sizeof(align_data)) - sizeof(node);
    ld_insert_last (new_liste, taille_data, (node *)current+1);
    current  = (align_data *)current + ((node *)current)->next;
  } while ( ((node *)current)->next ); 

  memmove ( ((head *)liste)->memory, ((head *)new_liste)->memory, size_of_liste);
  ((head *)liste)->first = ((head*)new_liste)->first;
  ((head *)liste)->last = ((head*)new_liste)->last;
  ((head *)liste)->libre = ((head*)new_liste)->libre;
  
  ld_destroy (new_liste);
  
  return liste;
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
  while( ld_total_useful_memory (liste) <= len)
    result = ld_add_memory (liste, 1000);
    
  return result;
}


static void *last_in_mem (void *liste){
  assert (((head*)liste)->first);

  ptrdiff_t max = ((head *)liste)->first;
  ptrdiff_t temp;
  void *n = (align_data *)(((head *)liste)->memory) + ((head*)liste)->first;

  while ( ((node *)n)->next != 0 ) {
    n = (align_data *)n + ((node *)n)->next;
    temp = (align_data *)n - (align_data *)((head *)liste)->memory;
    if (temp >max)
      max = temp;
  }

  return (align_data *)((head *)liste)->memory + max ;
}
