typedef struct hashset hashset;

struct hashset;

hashset* create_hashset();
void destroy_hashset(hashset*);

void insert(hashset*, double value);

double* to_array(hashset*, int*);
