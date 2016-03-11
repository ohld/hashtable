/*

 # hashtable source file
 #
 # Lang:     C
 # Author:   okhlopkov
 # Version:  0.1

 */
#include "hashtable.h"
#include "hashfunction.h"
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "logger/logger.h"

/* Private methods */

uint32_t hashtable_get_hash_from_key (hashtable_t *hashtable, char *key);
uint32_t hashtable_get_hash_from_pair (hashtable_t *hashtable, hashtable_pair_t *pair);
uint32_t hashtable_check_if_pair_is_good (hashtable_pair_t *pair);

/* Constructors */

hashtable_t *hashtable_create_table (uint32_t size) {
  assert(size > 0);

	hashtable_t *hashtable = (hashtable_t *)calloc(1, sizeof(hashtable_t));
	if (hashtable == NULL) {
		perror("Error while allocating memory for hashtable.");
		return NULL;
	}

	//initialising count and size fields.
	hashtable->count = 0;
	hashtable->size = size;

	//allocating memory for data.
	hashtable->arr = (hashtable_pair_t *)calloc(size, sizeof(hashtable_pair_t));
	if (hashtable->arr == NULL) {
		perror("Error while allocating memory for hashtable array.");
		return NULL;
	}

	//initializing all pairs with NULL.
	hashtable_pair_t *array = hashtable->arr;
	for (uint32_t i = 0; i < size; ++i) {
		array[i].key = NULL;
		array[i].value = NULL;
	}

	return hashtable;
}

hashtable_pair_t hashtable_create_pair (char *nkey, char *nvalue) {
	assert (nkey != NULL);
	assert (nvalue != NULL);

	//allocating memory for copies of key and value.
	char *key_copy = (char *) malloc (sizeof(char) * (strlen(nkey) + 1));
	char *value_copy = (char *) malloc (sizeof(char) * (strlen(nvalue) + 1));
	if (key_copy == NULL || value_copy == NULL) {
		perror ("Error while allocating memory for copy of key or value while creating pair.");
	}

	//copying key and value.
	key_copy = strcpy(key_copy, nkey);
	value_copy = strcpy(value_copy, nvalue);

	//creating pair.
	hashtable_pair_t a = {key_copy, value_copy};

	return a;
}

/* Destructor */

uint32_t hashtable_delete_table (hashtable_t *hashtable) {
	//checking arguments.
	assert (hashtable != NULL);
	assert (hashtable->arr != NULL);

	hashtable_pair_t *array = hashtable->arr;

	//freeing all keys-value pairs.
	uint32_t size = hashtable_size(hashtable);
	for (int i = 0; i < size; i++) {
		if ((array[i]).key != NULL) {
			free ((array[i]).key);
			free ((array[i]).value);
			(array[i]).key = NULL;
			(array[i]).value = NULL;
		}
	}

	free (array);
	hashtable->arr = NULL;
	free (hashtable);

	return 0;
}

/* Public properties */

uint32_t hashtable_is_empty (hashtable_t *hashtable) {
  assert (hashtable != NULL);
  return hashtable->count ? 0 : 1;
}

uint32_t hashtable_count (hashtable_t *hashtable) {
  assert (hashtable != NULL);
	return hashtable->count;
}

uint32_t hashtable_size (hashtable_t *hashtable) {
  assert (hashtable != NULL);
  return hashtable->size;
}

/* Public methods */
// Addtion
uint32_t hashtable_add_pair (hashtable_t *hashtable, hashtable_pair_t pair) {
	assert (hashtable != NULL);
	assert (hashtable_check_if_pair_is_good (&pair));

	//assigning to new pointer in order to write less code.
	hashtable_pair_t *array = hashtable->arr;

	//check whether memory is allocated and there is place for one more pair.
	if (array == NULL) return TABLE_NOT_CRTD;
	if (hashtable_count (hashtable) >= hashtable_size (hashtable)) return SIZE_EXCEEDED;

	//i is first found hash of the key.
	uint32_t i = hashtable_get_hash_from_pair (hashtable, &pair);

  /* // PART FOR DIFFERENT HASHTABLE IMPLEMENTATION
  //looking for first empty place to put new pair.
  uint32_t hashtbl_size = hashtable_size (hashtable);
	while ((array[i].key != NULL) && (strcmp((array[i]).key, pair.key) != 0)) {
		i += DEFAULT_STEPPING;
		if (i == hashtbl_size) i = 0;
	}

	//placing pair into array, incrementing counter.
	if (array[i].key == NULL) {
		array[i].key = pair.key;
		array[i].value = pair.value;
		hashtable->count ++;
	}
  */

  if (array[i].key == NULL) hashtable->count ++;
  array[i].key = pair.key;
  array[i].value = pair.value;

	//returning hash id.
	return i;
}

uint32_t hashtable_add_pair_by_key_value (hashtable_t *hashtable, char *key, char *value) {
  assert (hashtable != NULL);
  assert (key != NULL);
  assert (value != NULL);
  return hashtable_add_pair (hashtable, hashtable_create_pair (key, value));
}

// Getters
char *hashtable_get_value_by_key (hashtable_t *hashtable, char *key) {
	assert (hashtable != NULL);
	assert (key != NULL);

	//using array pointer in order to write less code.
	hashtable_pair_t *array = hashtable->arr;
	assert (array != NULL);

	//getting first hash from key and saving it for future use (in order not to count hash again).
	uint32_t i = hashtable_get_hash_from_key (hashtable, key);
	uint32_t hash = i;

	//getting size of hashtable.
	uint32_t hashtbl_size = hashtable_size (hashtable);

	//var to understand when we cycled through all table.
	uint32_t runned_through_cycle = 0;

	//looking for value.
	while ((array[i].key != NULL) && (strcmp((array[i]).key, key) != 0) && runned_through_cycle == 0) {
		i += DEFAULT_STEPPING;
		if (i == hashtbl_size) i = 0;
		if (i == hash) runned_through_cycle = 1;
	}

	//returning NULL in cases of not found.
	if (runned_through_cycle == 1) return NULL;
	if (array[i].key == NULL) return NULL;

	/* Creating copy of Value to return. */

	//asserting value in array is not NULL.
	assert ((array[i]).value != NULL);

	//allocating memory for new value instance. Size is incremented because of '\0' character.
	char *value_copy = (char *) malloc (sizeof(char) * (strlen((array[i]).value) + 1));

	//checking if memory is successfully allocated.
	if (value_copy == NULL) {
		perror("Error while allocating memory for value.");
		return NULL;
	}

	//copying value.
	value_copy = strcpy(value_copy, (array[i]).value);

	return value_copy;
}

char *hashtable_get_value_by_id (hashtable_t *hashtable, uint32_t hash_id) {
	assert (hashtable != NULL);
	assert (hash_id < hashtable_size(hashtable));

	//creating array pointer in order to write less code.
	hashtable_pair_t *array = hashtable->arr;

	//check if array memory is allocated and there is pair for hash_id.
	if (array == NULL) return NULL;
	if (array[hash_id].key == NULL) return NULL;

	/* Creating copy of Value. */

	//asserting value in array is not NULL.
	assert ((array[hash_id]).value != NULL);

	//allocating memory.
	char *value_copy = (char *) malloc (sizeof(char) * (strlen((array[hash_id]).value) + 1));

	//checking if memory is successfully allocated.
	if (value_copy == NULL) {
		perror("Error while allocating memory for value.");
		return NULL;
	}

	//copying value.
	value_copy = strcpy(value_copy, (array[hash_id]).value);

	return value_copy;
}

hashtable_pair_t *hashtable_get_pair_by_key (hashtable_t *hashtable, char *key) {

	/* Using get_value_by_key method then creating pair. */

	//retreiving value.
	char *nvalue = hashtable_get_value_by_key (hashtable, key);
	if (nvalue == NULL) return NULL;

	//copying key.
	char *key_copy = (char *) malloc (sizeof(char) * (strlen(key) + 1));
	if (key_copy == NULL) {
		perror ("Error while creating copy of key instance.");
		return NULL;
	}
	key_copy = strcpy(key_copy, key);

	//creating pair to return.
	hashtable_pair_t *pair = (hashtable_pair_t *)malloc(sizeof(hashtable_pair_t));
	if (pair == NULL) {
		perror ("Error while allocating memory for new pair.");
		return NULL;
	}
	pair->key = key_copy;
	pair->value = nvalue;

	return pair;

}

hashtable_pair_t *hashtable_get_pair_by_id (hashtable_t *hashtable, uint32_t hash_id) {
  assert(hashtable != NULL);
  assert(hashtable->arr != NULL);

	//using array pointer in order to write less code.
	hashtable_pair_t *array = hashtable->arr;

	//getting value.
	char *nvalue = hashtable_get_value_by_id (hashtable, hash_id);
	if (nvalue == NULL) return NULL;

	//getting key.
	char *key = (array[hash_id]).key;
	if (key == NULL) return NULL;

	//copying key instance.
	char *key_copy = (char *) malloc (sizeof(char) * (strlen(key) + 1));
	if (key_copy == NULL) {
		perror ("Error while creating key copy");
		return NULL;
	}
	key_copy = strcpy(key_copy, key);

	//creating pair to return.
	hashtable_pair_t *pair = (hashtable_pair_t *)malloc(sizeof(hashtable_pair_t));
	if (pair == NULL) {
		perror ("Error while allocating memory for new pair.");
		return NULL;
	}
	pair->key = key_copy;
	pair->value = nvalue;

	return pair;
}

// Deletion
uint32_t hashtable_delete_pair_by_key (hashtable_t *hashtable, char *key) {
	if (hashtable == NULL) return TABLE_NOT_CRTD;
  if (hashtable->arr == NULL) return TABLE_NOT_CRTD;
	assert (key != NULL);

	//using array pointer in order to write less code.
	hashtable_pair_t *array = hashtable->arr;

	/* Looking for pair in the same manner as when adding */
	uint32_t i = hashtable_get_hash_from_key (hashtable, key);
	uint32_t hash = i;
	uint32_t hashtbl_size = hashtable_size (hashtable);
	uint32_t runned_through_cycle = 0;

	while ((array[i].key != NULL) && (strcmp((array[i]).key, key) != 0) && runned_through_cycle == 0) {
		i += DEFAULT_STEPPING;
		if (i == hashtbl_size) i = 0;
		if (i == hash) runned_through_cycle = 1;
	}

	//if no pair found, return 1
	if (runned_through_cycle == 1) return 1;
	if (array[i].key == NULL) return 1;

	//freeing allocated memory and deleting.
	free(array[i].key);
	free(array[i].value);
	array[i].key = NULL;
	array[i].value = NULL;
	hashtable->count --;

	return 0;
}

uint32_t hashtable_delete_pair_by_id (hashtable_t *hashtable, uint32_t hash_id) {
	if (hashtable == NULL) return TABLE_NOT_CRTD;
	assert (hash_id < hashtable_size (hashtable));

	//using array pointer in order to write less code.
	hashtable_pair_t *array = hashtable->arr;
	if (array == NULL) return TABLE_NOT_CRTD;

	//if there is no pair by id, return 1.
	if (array[hash_id].key == NULL) return 1;

	//freeing allocated memory and deleting.
	free(array[hash_id].key);
	free(array[hash_id].value);
	array[hash_id].key = NULL;
	array[hash_id].value = NULL;
	hashtable->count --;

	return 0;
}

/* Private methods */

uint32_t hashtable_check_if_pair_is_good (hashtable_pair_t *pair) {
	assert (pair != NULL);
	assert (pair->key != NULL);
	assert (pair->value != NULL);
	return 1;
}

/* Hash functions */
uint32_t hashtable_get_hash_from_pair (hashtable_t *hashtable, hashtable_pair_t *pair) {
	assert (hashtable != NULL);
	assert (hashtable_check_if_pair_is_good (pair));
	return hashtable_get_hash_from_key (hashtable, pair->key);
}

uint32_t hashtable_get_hash_from_key (hashtable_t *hashtable, char *key) {
	assert (hashtable != NULL);
	assert (key != NULL);

	uint32_t hashval = hash_knuth(key);
	hashval %= hashtable_size(hashtable);

	return hashval;
}
