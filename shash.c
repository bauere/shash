/* shash: a simple hashtable implementation */
/* See LICENSE for license details. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _POSIX_C_SOURCE 200809L

struct HashTable {
	char **hashtable;
	unsigned int size;
};

unsigned int next_prime(unsigned int cur_prime);

int
init_table(struct HashTable *tbl, size_t size)
{
	tbl->hashtable = calloc(size, sizeof(char *));
	tbl->size = size;

	if (!(tbl->hashtable))
		return -1;
	return 0;
}

size_t
make_hash(struct HashTable *tbl, char *str)
{
	char c = str[0];
	size_t prehash = 0;
	size_t hash;

	for (int i = 0; c != EOF; i++) {
		c = str[i];
		prehash += c;
		prehash = prehash << 8;
	}
	
	hash = prehash % tbl->size;
	return hash;
}

unsigned int
add_value(struct HashTable *tbl, char *str)
{
	size_t hash = make_hash(tbl, str);
	
	if (tbl->hashtable[hash])
		return -1;

	tbl->hashtable[hash] = calloc(strlen(str), sizeof(char));
	strcpy(tbl->hashtable[hash], str);
	return hash;
}

/* Use in case of collision. */
int
resize_table(struct HashTable *newtbl, struct HashTable *oldtbl)
{
	unsigned int newsize = next_prime(oldtbl->size);

	if (init_table(newtbl, newsize) < 0)
		return -1;
	
	for (unsigned int i = 0; i < oldtbl->size; i++) {
		if (oldtbl->hashtable[i])
			add_value(newtbl, oldtbl->hashtable[i]);
	}
	
	return 0;	
} 

/* Use for size of new/resized hashtable. */
unsigned int
next_prime(unsigned int cur_prime)
{
	begin:
		cur_prime++;
		for (int i = 2; i < (cur_prime-1); i++) {
			if (!(cur_prime % i))
				goto begin;
		}

	return cur_prime;
}

/* Sample usage. */
int
main(int argc, char **argv)
{
	struct HashTable mytable;

	if (init_table(&mytable, STARTING_PRIME) < 0) {
		puts("Error creating hashtable.");
		return -1;
	}
	
	int hash = 0;
	if ((hash = add_value(&mytable, "TEST")) < 0) {
		puts("HASH COLLISION");
		return -1;	
	}

	printf("%s", mytable.hashtable[hash]);
	return 0;	
}
