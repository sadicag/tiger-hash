#include "main.h"

/**
 *  Hashes a complete file and returns back the hash.
 *    Returns back '1' if file has been hashed and printed.
 *    Returns back '0' if not.
 */
int hash_file(char* filename)
{  
  word64 plain_length;
  byte* plain = read_file_bytes(filename, &plain_length);

  if (plain != NULL)
  {
    word64 hash_length;
    byte* hash = tiger(plain, plain_length, &hash_length);
    // Print the values
    printbs(hash, hash_length);
    // Free the values 
    free(hash);
    return 1;
  }
  return 0;
}

int main(void)
{
  char filename_0[256] = "0.in";
  char filename_1[256] = "1.in";
  char filename_2[256] = "2.in";

  hash_file(filename_0);
  hash_file(filename_1);
  hash_file(filename_2);
  return EXIT_SUCCESS;
}

