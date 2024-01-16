#include "hashtable.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node* init_node() {
    Node* node = (Node*)malloc(sizeof(Node));
    assert(node != NULL);

    node->key = -1;
    node->next = NULL;

    return node;
}

HashTable* create_hashtable(int size) {
    assert(size > 0);

    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    if (table == NULL) {
        return NULL;
    }

    table->buckets = (Node**)malloc(sizeof(Node*) * size);
    if (table->buckets == NULL) {
        return NULL;
    }

    table->size = size;

    for (int i = 0; i < size; ++i) {
        // For each bucket, we include an empty head node for convenience
        Node* head = init_node();
        table->buckets[i] = head;
    }

    return table;
}

int free_hashtable(HashTable* table) {
    assert(table != NULL);

    for (int i = 0; i < table->size; ++i) {
        Node* curr = table->buckets[i];
        Node* next;
        while (curr != NULL) {
            next = curr->next;
            free(curr);
            curr = next;
        }
    }

    return 0;
}

Node* insert_hashtable(HashTable* table, int key) {
    assert(table != NULL);
    assert(key >= 0);

    int index = key % table->size;  // hash function is modulo operation
    Node* bucket = table->buckets[index];

    Node* curr = bucket->next;
    Node* prev = bucket;
    while (curr != NULL) {
        if (curr->key == key) {
            // Found a duplicate key, just announce failure
            return NULL;
        }
        prev = curr;
        curr = curr->next;
    }

    assert(prev != NULL);

    Node* new_node = init_node();
    new_node->key = key;
    prev->next = new_node;

    return new_node;
}

Node* lookup_hashtable(HashTable* table, int key) {
    assert(table != NULL);
    assert(key >= 0);

    int index = key % table->size;  // hash function is modulo operation
    Node* bucket = table->buckets[index];

    Node* curr = bucket->next;
    while (curr != NULL) {
        if (curr->key == key) {
            // Found a match
            return curr;
        }
        curr = curr->next;
    }

    return NULL;
}

int delete_hashtable(HashTable* table, int key) {
    assert(table != NULL);
    assert(key >= 0);

    int index = key % table->size;  // hash function is modulo operation
    Node* bucket = table->buckets[index];

    Node* curr = bucket->next;
    Node* prev = bucket;
    while (curr != NULL) {
        if (curr->key == key) {
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    assert(prev != NULL);

    if (curr == NULL) {
        // Could not find a matching key
        return -1;
    }

    // logical deletion
    prev->next = curr->next;
    curr->next = NULL;

    // phyisical deletion
    free(curr);

    return 0;
}

void print_hashtable(HashTable* table) {
    assert(table != NULL);

    for (int i = 0; i < table->size; ++i) {
        printf("bucket[%d]->", i);

        Node* bucket = table->buckets[i];
        Node* curr = bucket->next;
        while (curr != NULL) {
            printf("[%d]->", curr->key);
            curr = curr->next;
        }
        printf("(NULL)\n");
    }
}