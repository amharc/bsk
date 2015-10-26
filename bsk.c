#include "trie.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

char buffer[1000000];

void tokenize(struct trie *trie, char *restrict buffer) {
    char *token = strtok(buffer, " ");
    while(token) {
        ssize_t length = strlen(token);
        if(length > 0)
            trie_insert(trie, token, length);
        token = strtok(NULL, " ");
    }
}

int main() {
    while(fgets(buffer, sizeof(buffer) / sizeof(buffer[0]), stdin)) {
        struct trie *trie = trie_create();
        tokenize(trie, buffer);

        struct trie_get_even_response resp = trie_get_even(trie);
        if(resp.word) {
            printf("%s\n%s: %zd times\n", buffer, resp.word, resp.count);
        }
        trie_free(trie);
    }
}
