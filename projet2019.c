#include <stdio.h>

//crée toute les structure pour la liste
//@vovard
void *ld_creer (size_t nboctets){ 
  
}

//retourne le pointeur vers le premier noeud de la liste, ou NULL si la liste est vide
//@lemoinne
void *ld_first(void *liste){
  
}

//retourne le pointeur vers le dernier noeud de la liste, ou NULL si la liste est vide

void *ld_last(void *liste){ 

}

//current est un pointeur qui pointe soit vers head(i.e.liste==current) soit vers un nœud. Dans le premier cas,ld_next retourne la valeur de ld_first(liste). Dans le deuxième cas elle retourne le pointeur vers le nœud qui suit current sur la liste. Si current pointe vers le dernier nœud sur la liste, la fonction retourne NULL. Si la liste est vide et liste==current, la fonction retourne NULL

void *ld_next(void *liste, void *current){ 

}

//current est un pointeur qui pointe soit vers head(i.e.liste==current) soit vers un nœud. Dans le premier cas,ld_previous retourne la valeur de ld_last(liste). Dans le deuxième cas elle retourne le pointeur vers le nœud qui precede current sur la liste. Si current pointe vers le premier nœud sur la liste, la fonction retourne NULL. Si la liste est vide et liste==current, la fonction retourne NULL

void *ld_previous(void *liste, void *current){

}

//la fonction detruit la liste en libérant la mémoire de head et de memory

void ld_destroy(void *liste){

}

//current est un pointeur vers un nœud (donc différent deliste).ld_get copie len octets du champ data du nœud current vers l’adresse val. Si le nombre d’octets demandé est supérieur au nombre d’octets dans le champ data du nœud, on limite la copie aux octets qui sont dansdata. La fonction retourne le nombre d’octets copiés.

size_t ld_get(void *liste, void *current, size_t len, void *val){

}

//insère un nouveau nœud comme le premier élément de la liste

void * ld_insert_first(void *liste, size_t len, void *p_data){

}

//insère un nouveau nœud comme le dernier élément de la liste

void * ld_insert_last(void *liste, size_t len, void *p_data){

}

//insère un nouveau nœud dans la liste. Les paramètres len et p_data sont les mêmes que dans la fonction ld_insert_first. n est un pointeur vers un nœud, ld_insert_before insère le nouveau nœud avant le nœud n.

void * ld_insert_before(void*liste, void*n, size_t len, void*p_data){

}

//supprime le noeud pointé par n. La fonction retourne liste quand la suppression réussit et NULL sinon.

void * ld_delete_node(void*liste, void*n){

}

//retourne le nombre d’octets libres dans la mémoire memory. Cela permettra de voir si la mémoire commence à être saturée

size_t  ld_total_free_memory(void*liste){

}

//retourne la taille de la mémoire libre qui peut être encore utilisée pour créer de nouveauxnœuds (si on ne réutilise pas la mémoire libérée cela peut être beaucoup moins que ce queretourne la fonction précédente qui compte aussi la mémoire libre mais non utilisable).

size_t  ld_total_useful_memory(void*liste){

}

//agrandit la mémoire de nb_octets. Comme pour la création de la liste, on arrondit nb_octets vers un multiple de sizeof(align_data). Il est impossible de diminuer la taille de mémoire. La fonction retourne NULL en cas deproblème et liste sinon.

void*ld_add_memory(void*liste, size_t nboctets){

}

//compacte la liste en mettant les nœuds au début de la mémoire avec une seule tranche de blocs libres à la fin de la mémoire.(La fonction devra sans doute allouer une nouvelle mémoire (de la même taille que l’ancienne) pour y recopier tous les nœuds l’un après l’autre dans l’ordre de parcours sur la liste.) La fonction retourne NULL en cas de problème (échec de malloc), sinon elle retourne liste.

void*ld_compactify(void*liste){

}
