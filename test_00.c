#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

#include "projet2019.h"
#include "affichage.h"

#define NBR_TYPE 4

typedef enum data_type { NBR_INT, NBR_DOUBLE, CHARACTERE, STRING, ANY} data_type;  

static double puissance (double, int);
size_t random_var (data_type, void*);
static int lancement_sequence_test ();
int ajout_noeuds (void *, data_type, size_t);
int lancement_sequence_action (void *, size_t, int);

int main(){
  srand (time (NULL));
  int err = lancement_sequence_test ();

  if (err != 0)
    affichage_erreur (err);
  
  return 0;
}


/*Lance une séquence de nbr-test tests. t definie le type de donnée à utilisé (ANY permet d'utiliser tous les types de donnée definie de facons aleatoire).
taille_liste definie la taille des listes (si 0, alors la taille est aleatoire mais <TAILLE_MAX_LISTE.
taille_mem_init definie la taille initial de la memoire des listes (si 0, alors aleatoire. 
return 0 si les tests sont ok*/
static int lancement_sequence_test () {
  void *liste;
  char c;
  int freq, nbr_action, res;
  
  //LISTE DE INT
  printf ("LISTE DE INT\n\n");
  liste = ld_create (10);
  if (liste == NULL)
    return 1;  
  res = ajout_noeuds (liste, NBR_INT, 10);
  if (res !=0){
    ld_destroy (liste);
    return res;
  }
  res = affichage_noeuds_int (liste);
  if (res != 0){
    ld_destroy (liste);
    return res;
  }
  print_node_liste (liste);
  ld_destroy(liste);
  
  //LISTE DE DOUBLE
  printf("\n\nLISTE DE DOUBLE\n\n");
  liste = ld_create (100);
  if (liste == NULL)
    return 1;  
  res = ajout_noeuds (liste, NBR_DOUBLE, 10);
  if (res !=0){
    ld_destroy (liste);
    return res;
  }
  res = affichage_noeuds_double (liste);
  if (res != 0){
    ld_destroy (liste);
    return res;
  }
  print_node_liste (liste);
  ld_destroy(liste);

  //LISTE DE CHAR
  printf("\n\nLISTE DE CHAR\n\n");
  liste = ld_create (100);
  if (liste == NULL)
    return 1;  
  res = ajout_noeuds (liste, CHARACTERE, 10);
  if (res !=0){
    ld_destroy (liste);
    return res;
  }
  res = affichage_noeuds_char (liste);
  if (res != 0){
    ld_destroy (liste);
    return res;
  }
  print_node_liste (liste);
  ld_destroy(liste);

  //LISTE DE STRING
  printf("\n\nLISTE DE STRING\n\n");
  liste = ld_create (100);
  if (liste == NULL)
    return 1;  
  res = ajout_noeuds (liste, STRING, 10);
  if (res !=0){
    ld_destroy (liste);
    return res;
  }
  res = affichage_noeuds_string (liste);
  if (res != 0){
    ld_destroy (liste);
    return res;
  }
  print_node_liste (liste);
  ld_destroy(liste);

  
  printf("\nVoulez-vous lancer la séquence d'actions aléatoires (insertions/suppressions) ? (y/n)\n");
  scanf("%c", &c);
  if (c == 'y') {
    liste = ld_create (100);
    if (liste == NULL)
      return 1;  
    int res = ajout_noeuds (liste, ANY, 50);
    if (res !=0){
      ld_destroy (liste);
      return res;
    }
    printf("Entrez la fréquence d'insertion : \n");
    scanf("%d", &freq);
    printf("Entrez le nombre d'actions à éffectué : \n");
    scanf("%d", &nbr_action);
    res = lancement_sequence_action (liste, nbr_action, freq);
    if (res != 0) {
      ld_destroy(liste);
      return res;
    }
    
    printf("\n\nLANCEMENT DE COMPACTIFY\nmémoire libre : %lu\n", ld_total_free_memory(liste));
    printf("mémoire utilisable avant compactify : %lu\n", ld_total_useful_memory(liste));
    void *temp = ld_compactify (liste);
    if ( temp == NULL) {
      ld_destroy (liste);
      return 6;
    }
    printf("mémoire utilisable aprés compactify : %lu\n", ld_total_useful_memory(liste));

    printf("\nVoulez-vous afficher la liste ? (y/n)\n");
    scanf("%c", &c);
    while (c != 'y' && c != 'n')
      scanf("%c", &c);
    if (c == 'y')
      print_node_liste (liste);
    
    ld_destroy (liste);
  }
  return 0;
}


  
size_t random_var (data_type t, void *dest) {
  void *p;
  size_t taille;
  int i, pourc_al_da;
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
    c = (char)(rand()%93)+33;
    p = &c;
    taille = sizeof(char);
    break;
    
  case 3 : //string
    pourc_al_da = 100/sizeof(align_data);
    taille = (rand()%pourc_al_da)+1;
    taille *= sizeof(align_data);
    s = realloc (s, taille*sizeof(char));

    for (int i = 0; i<taille-1; i++)
      random_var (2, s+i);
    
    s[taille-1]='\0';
    p = s; 
    taille = taille*sizeof(char);
    break;
    
  default :
    taille = random_var (rand()%NBR_TYPE, dest);
    free(s);
    return taille;
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
  void *node;
  void *data = malloc (sizeof(char)*100);
  size_t size_data;
  
  if(taille_liste !=0) {
    size_data = random_var(t, data);
    node = ld_insert_first (liste, size_data, data);
    if( node == NULL){
      free(data);
      return 8;
    }
  }
  
  for (int i = 1; i<taille_liste; i++) {
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


int lancement_sequence_action (void *liste, size_t nbr_op, int freq_inser) {
  int oper, pos;
  void *n = liste;
  void *data = malloc (100*sizeof(char));
  size_t taille_data;
  
  for (int i = 0; i<nbr_op; i++) {
    printf ("action %d\n", i);
    oper = rand() % freq_inser;
    pos = rand() % (((head *)liste)->nbr_noeud -1);
    pos ++;

    for (int j = 0; j<pos; j++)
      n = ld_next (liste, n);
    
    if (oper) {
      printf("insertion\n");
      taille_data = random_var (ANY, data);
      n = ld_insert_after (liste, n, taille_data, data);
      if (n == NULL) {
	free(data);
	return 11;
      }
      
    } else {
      printf ("deletion\n");
      n = ld_delete_node (liste, n);
      if (n == NULL){
	free (data);
	return 12;
      }
      
    }

    n = liste;
  }
  free (data);
  return 0;
}
