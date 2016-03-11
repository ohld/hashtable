#include "hashtable.h"
#include <stdio.h>

int main () {
	/* Generating array of random strings to test */
	char **array_of_strings = (char **)malloc(sizeof(char *) * 300);
	for (int i = 0; i < 300; i++) {
		uint32_t size_of_str = arc4random() % 1000 + 2;
		array_of_strings[i] = (char *)malloc(sizeof(char) * size_of_str);
		for (int j = 0; j < size_of_str - 1; j ++) {
			array_of_strings[i][j] = 32 + arc4random() % 93; //generating random char
		}
		array_of_strings[i][size_of_str - 1] = 0;
		//printf ("%s\t", array_of_strings[i]);
	}
	//printf ("\n");

	/* First table tests: */

	uint32_t hashtable1_size = 10;
	hashtable_t *tsttbl1 = hashtable_create_table (hashtable1_size);
	for (int i = 0; i < hashtable1_size + 1; i++) {
    hashtable_add_pair_by_key_value (tsttbl1,
																	  array_of_strings[i],
																	  array_of_strings[i + hashtable1_size + 1]);
	}
	hashtable_delete_pair_by_key (tsttbl1, array_of_strings[hashtable1_size - 1]);
	hashtable_count (tsttbl1);
	hashtable_delete_table(tsttbl1);

	/* Second table tests */

	uint32_t hashtable2_size = 300;
	hashtable_t *tsttbl2 = hashtable_create_table (hashtable2_size);
	char *key0 = array_of_strings[0];
	char *value0 = array_of_strings[1];
	char *key1 = array_of_strings[2];
	char *value1 = array_of_strings[3];
	char *key2 = array_of_strings[4];
	char *value2 = array_of_strings[5];

	uint32_t id0 = hashtable_add_pair_by_key_value (tsttbl2, key0, value0);
	uint32_t id1 = hashtable_add_pair_by_key_value (tsttbl2, key1, value1);
	uint32_t id2 = hashtable_add_pair_by_key_value (tsttbl2, key2, value2);

	char *getted_value0 = hashtable_get_value_by_key (tsttbl2, key0);
	char *getted_value1 = hashtable_get_value_by_key (tsttbl2, key1);
	char *getted_value2 = hashtable_get_value_by_key (tsttbl2, key2);

	char *getted_value_by_id0 = hashtable_get_value_by_id (tsttbl2, id0);
	char *getted_value_by_id1 = hashtable_get_value_by_id (tsttbl2, id1);
	char *getted_value_by_id2 = hashtable_get_value_by_id (tsttbl2, id2);

	if (strcmp (value0, getted_value0) != 0 ||
		strcmp (value0, getted_value_by_id0) != 0 ||
		strcmp (value1, getted_value1) != 0 ||
		strcmp (value1, getted_value_by_id1) != 0 ||
		strcmp (value2, getted_value2) != 0 ||
		strcmp (value2, getted_value_by_id2) != 0) {
		printf ("\n\nERROR: wrong values in table.\n");
		hashtable_delete_table(tsttbl2);
		return 1;
	}

	// Checking empty property.
	int ok = 1;
	if (hashtable_is_empty (tsttbl2) == 1) ok = 0;
	hashtable_delete_table(tsttbl2);
	tsttbl2 = hashtable_create_table (hashtable2_size);
	if (hashtable_is_empty (tsttbl2) == 0) ok = 0;

	if (ok != 1) {
		printf ("\n\nERROR: Test on empty method was failed.\n");
		hashtable_delete_table (tsttbl2);
		return 1;
	}

	/* Checking size method */
	if (hashtable_size (tsttbl2) != hashtable2_size) {
		printf ("\n\nERROR: Test on size method was failed.\n");
		hashtable_delete_table (tsttbl2);
		return 1;
	}

	/* Deleting table. */
	hashtable_delete_table(tsttbl2);

	printf ("\n\nAll tests completed successfully.\n");
	return 0;
}
