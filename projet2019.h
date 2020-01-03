#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#ifndef PROJET2019_H
#define PROJET2019_H


typedef union align_data{
  intmax_t a;
  void *adr;
  long double c;
} align_data;
 
typedef struct node{
  ptrdiff_t next;
  ptrdiff_t previous;
  size_t len;
  align_data data[];
} node;

typedef struct head{
  void *memory;   //pointeur vers la mémoire
  ptrdiff_t first;
  ptrdiff_t last;
  ptrdiff_t libre;
  size_t len;
  size_t nbr_noeud;
} head;

void *ld_create (size_t);                                                //1

void *ld_first (void *liste);                                            //2
void *ld_last (void *liste);                                             //3

void *ld_next (void *liste, void *current);                              //4
void *ld_previous (void *liste, void *current);                          //5

void ld_destroy (void *liste);                                           //6

size_t ld_get (void *liste, void *current, size_t len, void *vl);        //7

void *ld_insert_first (void *liste, size_t len, void *p_data);           //8
void *ld_insert_last (void *liste, size_t len, void *p_data);            //9
void *ld_insert_before (void *liste, void *n, size_t len, void *p_data); //10
void *ld_insert_after (void *liste, void *n, size_t len, void *p_data);  //11

void *ld_delete_node (void *liste, void *n);                             //12

size_t ld_total_free_memory (void *liste);                               //13
size_t ld_total_useful_memory (void *liste);                             //14

void *ld_add_memory (void *liste, size_t nboctets);                      //15
void *ld_compactify (void*liste);                                        //16

#endif
