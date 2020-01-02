#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

#include "projet2019.h"
#include "affichage.h"

#define TAILLE_MAX_LISTE 1000
#define NBR_TYPE 4

typedef enum data_type { NBR_INT, NBR_DOUBLE, CHARACTERE, STRING, ANY} data_type;  

static double puissance (double, int);
size_t random_var (data_type, void*);
static int lancement_sequence_test (int, data_type, size_t, size_t);
int ajout_noeuds (void *, data_type, size_t);

int main(){
  srand (time (NULL));
  int err = lancement_sequence_test (1, NBR_INT, 10, 10);

  if (err != 0)
    affichage_erreur (err);
  
  return 0;
}


/*Lance une séquence de nbr-test tests. t definie le type de donnée à utilisé (ANY permet d'utiliser tous les types de donnée definie de facons aleatoire).
taille_liste definie la taille des listes (si 0, alors la taille est aleatoire mais <TAILLE_MAX_LISTE.
taille_mem_init definie la taille initial de la memoire des listes (si 0, alors aleatoire. 
return 0 si les tests sont ok*/
static int lancement_sequence_test (int nbr_test, data_type t, size_t taille_liste, size_t taille_mem_init) {
  assert (nbr_test>0);
  void *liste;
  
  
  for (int i = 0; i<nbr_test; i++) {
    
    //creation de liste
    if(taille_liste)
      liste = ld_create (taille_mem_init);
    else
      liste = ld_create ((rand()%10000)+1);
    if (liste == NULL) {
      return 1;
    }
    printf ("CREATION DE LA LISTE -> OK\n");
    
    int res = ajout_noeuds (liste, t, taille_liste);
    if (res !=0){
      ld_destroy (liste);
      return res;
    }

    res = affichage_noeuds_int (liste);
    if (res != 0){
      ld_destroy (liste);
      return res;
    }
    
    ld_destroy (liste);
  } 
  return 0;
}


  
size_t random_var (data_type t, void *dest) {
  void *p;
  size_t taille;
  int i;
  double d;
  char c;
  char *s = malloc (sizeof(char));
  
  switch (t) {
  case 0 : //int
    i = rand();
    if (rand()%2)
      i = -1*i;
    p = &i;
    taille = sizeof(int);
    break;

  case 1 : //double
    d = (double)rand() * puissance (0.1, rand()%36);
    if (rand()%2)
      d = -1*d;
    p = &d;
    taille = sizeof(double);
    break;

  case 2 : //char
    c = (char)rand();
    p = &c;
    taille = sizeof(char);
    break;
    
  case 3 : //string
    taille = (rand()%100)+1;
    s = realloc (s, taille*sizeof(char));

    for (int i = 0; i<taille-1; i++)
      random_var (2, s+i);
    
    s[taille-1]='\0';
    p = s; 
    taille = taille*sizeof(char);
    break;

  default :
    taille = random_var (rand()%NBR_TYPE, dest);
  }

  void * temp = memcpy (dest, p, taille);
  free (s);
  if (temp != NULL)
    return taille;
  else
    printf("error");

  return 0;
}



static double puissance (double x, int n) {
  if ( n==0 )
    return 1;
  else
    return x*puissance (x, n-1);
}


int ajout_noeuds (void *liste, data_type t, size_t taille_liste) {
  size_t size_l;
  void *node;
  void *data = malloc (sizeof(char)*100);
  size_t size_data;
  
  if (taille_liste == 0)
    size_l = rand()%TAILLE_MAX_LISTE;
  else
    size_l = taille_liste;
  
  if(size_l !=0) {
    size_data = random_var(t, data);
    node = ld_insert_first (liste, size_data, data);
    if( node == NULL){
      free(data);
      return 8;
    }
  }
  
  for (int i = 1; i<size_l; i++) {
    size_data = random_var(t, data);
    node = ld_insert_after (liste, node, size_data, data);
    if( node == NULL){
      free(data);
      return 11;
    }
  }

  free(data);
  return 0;
}
