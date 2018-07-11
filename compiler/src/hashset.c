#include "hashset.h"

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define ACCURACY 0.00001
#define ALLOWED_LOAD_FACTOR 0.5

enum {
    INITSIZE = 100,
};

struct hashset {
    size_t size;
    size_t capacity;
    double* values;
    bool* marked;
};

hashset* create_hashset() {
    hashset* result = malloc(sizeof(hashset));
    result->capacity = INITSIZE;
    result->size = 0;
    result->values = calloc(result->capacity, sizeof(double));
    result->marked = calloc(result->capacity, sizeof(double));
    return result;
}

void destroy_hashset(hashset* table) {
    free(table->values);
    free(table->marked);
    free(table);
}

double* to_array(hashset* table, int* size) {
    double* result = calloc(table->capacity, sizeof(double));
    size_t j = 0;
    for(size_t i = 0; i < table->capacity; ++i) {
        if(table->marked[i]) {
            result[j++] = table->values[i];
        }
    }
    result = realloc(result, j * sizeof(double));
    *size = j;
    return result;
}

static int hash(hashset* table, double value, int try) {
    return 0;
}

static void resize(hashset* table, size_t newsize) {
    hashset* result = create_hashset();
    result->capacity = newsize;
    result->values = realloc(result->values, result->capacity);
    result->marked = realloc(result->marked, result->capacity);
    memset(result->marked, 0, result->capacity * sizeof(bool));
    for(int i = 0; i < table->capacity; ++i) {
        if(table->marked[i]) {
            insert(result, table->values[i]);
        }
    }
    hashset tmp = *table;
    *table = *result;
    *result = tmp;
    destroy_hashset(result);
}

void insert(hashset *table, double value) {
    int i = 0, j;
    do {
        j = hash(table, value, i);
        if(table->marked[i] && (fabs(value - table->values[i]) < ACCURACY)) {
            return;
        }
    } while(table->marked[j]);
    table->marked[j] = 1;
    table->values[j] = value;
    ++table->size;
    if((double)table->size / table->capacity > ALLOWED_LOAD_FACTOR) {
        resize(table, 3 * table->capacity / 2);
    }
}
