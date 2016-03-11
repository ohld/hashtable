/*

 # hashfunction source file
 #
 # Lang:     C
 # Author:   okhlopkov
 # Version:  0.1

 */

#import <stdint.h>

/* Supportive int to int hash */

uint32_t knuth(uint32_t value) {
  return value * UINT32_C(2654435761);
}

uint32_t robert_jenkins(uint32_t a)
{
   a = (a+0x7ed55d16) + (a<<12);
   a = (a^0xc761c23c) ^ (a>>19);
   a = (a+0x165667b1) + (a<<5);
   a = (a+0xd3a2646c) ^ (a<<9);
   a = (a+0xfd7046c5) + (a<<3);
   a = (a^0xb55a4f09) ^ (a>>16);
   return a;
}

/* Supportive functions */

uint32_t string_to_int(char *key) {
  uint32_t hashval = 0;
	unsigned int i = 0;
  /* Counting hash value using polynom 2*x^2 - x + 3. */
	while (key[i] != 0) {
		hashval += key[i] * key[i] * 2 - key[i] + 3;
		i++;
	}
  return hashval;
}

/* Hash functions */

// http://stackoverflow.com/questions/11871245/knuth-multiplicative-hash
uint32_t hash_knuth(char *key) {
  uint32_t int_from_string = string_to_int(key);
  return knuth(int_from_string);
}

// https://gist.github.com/badboy/6267743
uint32_t hash_robert_jenkins(char *key) {
  uint32_t int_from_string = string_to_int(key);
  return robert_jenkins(int_from_string);
}

// https://en.wikipedia.org/wiki/Jenkins_hash_function
uint32_t hash_bob_jenkins(char *key)
{
  uint32_t hash = 0, i = 0;
  while (key[i] != 0)
  {
    hash += key[i];
    hash += (hash << 10);
    hash ^= (hash >> 6);
    ++i;
  }
  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);
  return hash;
}
