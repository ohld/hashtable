#include "hashtable.h"
#include <stdio.h>

int main () {
	uint32_t hashtable1_size = 300;
	// /* Generating array of random strings to test */
	char **array_of_strings = (char **)malloc(sizeof(char *) * hashtable1_size);
	for (int i = 0; i < hashtable1_size; i++) {
		uint32_t size_of_str = arc4random() % 50 + 1;
		array_of_strings[i] = (char *)malloc(sizeof(char) * size_of_str);
		for (int j = 0; j < size_of_str - 1; j ++) {
			array_of_strings[i][j] = 33 + arc4random() % 92; //generating random char

		}
		array_of_strings[i][size_of_str - 1] = 0;
		//printf ("%s\t", array_of_strings[i]);
	}

	hashtable_t *tsttbl1 = hashtable_create_table (hashtable1_size);
	for (int i = 0; i < hashtable1_size + 1; i++) {
    hashtable_add_pair_by_key_value (tsttbl1,
																	  array_of_strings[i],
																	  array_of_strings[i + 1]);
	}
	hashtable_delete_pair_by_key (tsttbl1, array_of_strings[hashtable1_size - 1]);
	hashtable_count (tsttbl1);

	char *key0 = array_of_strings[0];
	char *value0 = array_of_strings[1];
	char *key1 = array_of_strings[2];
	char *value1 = array_of_strings[3];
	char *key2 = array_of_strings[4];
	char *value2 = array_of_strings[5];

	uint32_t id0 = hashtable_add_pair_by_key_value (tsttbl1, key0, value0);
	uint32_t id1 = hashtable_add_pair_by_key_value (tsttbl1, key1, value1);
	uint32_t id2 = hashtable_add_pair_by_key_value (tsttbl1, key2, value2);

	char *getted_value0 = hashtable_get_value_by_key (tsttbl1, key0);
	char *getted_value1 = hashtable_get_value_by_key (tsttbl1, key1);
	char *getted_value2 = hashtable_get_value_by_key (tsttbl1, key2);

	char *getted_value_by_id0 = hashtable_get_value_by_id (tsttbl1, id0);
	char *getted_value_by_id1 = hashtable_get_value_by_id (tsttbl1, id1);
	char *getted_value_by_id2 = hashtable_get_value_by_id (tsttbl1, id2);

	if (strcmp (value0, getted_value0) != 0 ||
		strcmp (value0, getted_value_by_id0) != 0 ||
		strcmp (value1, getted_value1) != 0 ||
		strcmp (value1, getted_value_by_id1) != 0 ||
		strcmp (value2, getted_value2) != 0 ||
		strcmp (value2, getted_value_by_id2) != 0) {
		printf ("\n\nERROR: wrong values in table.\n");
		hashtable_delete_table(tsttbl1);
		return 1;
	}

	// Checking empty property.
	if (hashtable_is_empty (tsttbl1)) {
		printf ("\n\nERROR: Test on empty computed property was failed.\n");
		hashtable_delete_table (tsttbl1);
		return 1;
	}

	/* Checking size method */
	if (hashtable_size (tsttbl1) != hashtable1_size) {
		printf ("\n\nERROR: Test on size method was failed.\n");
		hashtable_delete_table (tsttbl1);
		return 1;
	}

	// /* Deleting table. */
	hashtable_delete_table (tsttbl1);

	printf ("\n\nAll tests completed successfully.\n");
	return 0;
}
