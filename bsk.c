#include "trie.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct state {
    struct unbounded_string *line;
    struct trie *trie;
    size_t word_begin;
};

struct state create_state() {
    struct state result = {
        .line = us_from_string(""),
        .trie = trie_create(),
        .word_begin = 0,
    };

    return result;
}

void cleanup_state(struct state *state) {
    us_free(state->line);
    trie_free(state->trie);
}

void push_word(struct state *state) {
    size_t current = us_length(state->line);
    if(state->word_begin == current)
        return;

    trie_insert(state->trie, us_to_string(state->line) + state->word_begin, current - state->word_begin);
    state->word_begin = current + 1;
}

int main() {
    while(!feof(stdin)) {
        struct state __attribute__((cleanup(cleanup_state))) state = create_state();

        while(true) {
            char c = getchar();
            if(c == '.')
                return 0;
            else if(c == '\n' || c == EOF)
                break;
            else if(isspace(c)) {
                if(state.word_begin == us_length(state.line))
                    ++state.word_begin;
                else
                    push_word(&state);
            }
            us_push(state.line, c);
        }

        push_word(&state);

        struct trie_get_even_response response = trie_get_even(state.trie);
        if(response.word) {
            /* null-terminate the string */
            us_push(state.line, 0);
            printf("%s\n%s: %zd times\n", us_to_string(state.line), response.word, response.count);
            free(response.word);
        }
    }

    return 0;
}
