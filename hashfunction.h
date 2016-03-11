/*

 # hashfunction header file
 #
 # Lang:     C
 # Author:   okhlopkov
 # Version:  0.1

 */

#import <stdint.h>

/* String hash functions */
uint32_t hash_knuth(char *key);
uint32_t hash_robert_jenkins(char *key);
uint32_t hash_bob_jenkins(char *key);

/* Int to int hash */
uint32_t knuth(uint32_t value);
uint32_t robert_jenkins(uint32_t a);
