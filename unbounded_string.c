#include "unbounded_string.h"
#include "common.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct unbounded_string {
    char *data;
    size_t capacity, length;
};

struct unbounded_string* us_from_string(const char *string) {
    struct unbounded_string *result = calloc(1, sizeof(struct unbounded_string));
    if(!result)
        fail("Unable to allocate memory for an unbounded string");

    result->data = strdup(string);
    if(!result->data)
        fail("Unable to copy a string");

    result->capacity = result->length = strlen(string);

    return result;
}


void us_free(struct unbounded_string *us) {
    free(us->data);
    free(us);
}

const char* us_to_string(const struct unbounded_string *us) {
    return us->data;
}

void us_push(struct unbounded_string *us, char ch) {
    if(us->capacity == us->length) {
        us->capacity = 2 * us->capacity + 1;
        us->data = realloc(us->data, us->capacity);
        
        if(!us->data)
            fail("Unable to grow an unbounded string");
    }

    us->data[us->length] = ch;
    us->length++;
}

void us_pop(struct unbounded_string *us) {
    assert(us->length > 0);
    us->length--;
}

size_t us_length(struct unbounded_string *us) {
    return us->length;
}
