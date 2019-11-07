#ifndef projet2019_h
#define projet2019_h


typed struct {
  void *memory;   //pointeur vers la mémoire
  ptrdiff_t first;
  ptrdiff_t last;
  ptrdiff libre;
} head;


void *ld_creer (size_t);

void *ld_first (void *liste);
void *ld_last (void *liste);

void *ld_next (void *liste, void *current);
void *ld_previous (void *liste, void *current);

void ld_destroy (void *liste);

size_t ld_get (void *liste, void *current, size_t len, void *vl);

static void *ld_create_node (size_t len, void *p_data);

void *ld_insert_first (void *liste, size_t len, void *p_data);
void *ld_insert_last (void *liste, size_t len, void *p_data);
void *ld_insert_before (void *liste, void *n, size_t len, void *p_data);
void *ld_insert_after (void *liste, void *n, size_t len, void *p_data);

size_t ld_total_free_memory (void *liste);
size_t ld_total_useful_memory (void *liste);

void *ld_add_memory (void *liste, size_t nboctets);
void *ld_copactify (void*liste);

