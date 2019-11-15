#ifndef PROJET2019_H
#define PROJET2019_H


typedef union align_data;
typedef struct node;
typedef struct head;


void *ld_create (size_t);

void *ld_first (void *liste);
void *ld_last (void *liste);

void *ld_next (void *liste, void *current);
void *ld_previous (void *liste, void *current);

void ld_destroy (void *liste);

size_t ld_get (void *liste, void *current, size_t len, void *vl);

void *ld_insert_first (void *liste, size_t len, void *p_data);
void *ld_insert_last (void *liste, size_t len, void *p_data);
void *ld_insert_before (void *liste, void *n, size_t len, void *p_data);
void *ld_insert_after (void *liste, void *n, size_t len, void *p_data);

void *ld_delete_node (void *liste, void *n);

size_t ld_total_free_memory (void *liste);
size_t ld_total_useful_memory (void *liste);

void *ld_add_memory (void *liste, size_t nboctets);
void *ld_copactify (void*liste);

#endif
