#include "hashset.h"

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define ACCURACY 0.00001
#define ALLOWED_LOAD_FACTOR 0.5

enum {
    INITSIZE = 4,
};

struct hashset {
    size_t size;
    size_t capacity;
    double* values;
    bool* marked;
};

hashset* create_hashset() {
    hashset* result = calloc(1, sizeof(hashset));
    result->capacity = INITSIZE;
    result->size = 0;
    result->values = calloc(result->capacity, sizeof(double));
    result->marked = calloc(result->capacity, sizeof(bool));
    return result;
}

void destroy_hashset(hashset* table) {
    free(table->values);
    free(table->marked);
    free(table);
}

double* to_array(hashset* table, size_t* size) {
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
    union {
        long long l;
        double d;
    } u;
    u.d = value;
    long long dhash = u.l;
    return (2*dhash + try + try * try)/2 % table->capacity;
}

static void resize(hashset* table, size_t newsize) {
    hashset* result = create_hashset();
    result->capacity = newsize;
    result->values = realloc(result->values, result->capacity * sizeof(double));
    result->marked = realloc(result->marked, result->capacity * sizeof(bool));
    memset(result->marked, 0, result->capacity * sizeof(bool));
    for(int i = 0; i < table->capacity; ++i) {
        if(table->marked[i]) {
            insert(result, table->values[i]);
        }
    }
    destroy_hashset(table);
    *table = *result;
}

void insert(hashset *table, double value) {
    int i = 0, j;
    do {
        j = hash(table, value, i++);
        if(table->marked[j] && (fabs(value - table->values[j]) < ACCURACY)) {
            return;
        }
    } while(table->marked[j]);
    table->marked[j] = 1;
    table->values[j] = value;
    ++(table->size);
    if((double)table->size / table->capacity > ALLOWED_LOAD_FACTOR) {
        resize(table, 2 * table->capacity);
    }
}

void append(hashset *table, hashset *appended) {
    for(size_t i = 0; i < appended->capacity; ++i) {
        if(appended->marked[i]) {
            insert(table, appended->values[i]);
        }
    }
}
