#include <stddef.h>

typedef struct hashset hashset;

struct hashset;

hashset* create_hashset();
void destroy_hashset(hashset*);

void insert(hashset*, double);

void append(hashset*, hashset*);

double* to_array(hashset*, size_t*);
