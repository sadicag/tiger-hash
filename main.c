#include <stdio.h>
#include <stdlib.h>

#include "tiger/tiger.h"

/**
 *  The state of the program
 */
typedef struct _program_state {
  byte* input_path;
  byte* output_path;
  int is_hex;
} program_state;

/**
 *  Hashes a complete file and prints/saves according to program state.
 *    Returns back '1' if there was no error.
 *    Returns back '0' if not.
 */
int hash_file(program_state* state)
{  
  word64 plain_length;
  byte* plain = read_file_bytes(state->input_path, &plain_length);

  if (plain != NULL)
  {
    word64 hash_length;
    byte* hash = tiger(plain, plain_length, &hash_length);
    
    // Print the values if no output_path
    if (state->output_path == NULL)
    {
      if (state->is_hex)
      {
        printxs(hash, hash_length);
      } else {
        printbs(hash, hash_length);
      }
    } else {
      // If there is an output path, write into it
      write_to_file(state->output_path, hash, hash_length, state->is_hex);
    }
    
    // Free the values 
    free(hash);
    return 1;
  }
  return 0;
}

/**
 *  Print the arguments for the tiger-hash program.
 */
void print_help()
{
  printf("Usage:\n");
  printf("\ttiger-hash [options] <file>\n");
  printf("\n");
  printf("Hash a file according to Tiger algorithm created by Ross Anderson and Eli Biham.\n");
  printf("\n");
  printf("Options:\n");
  printf("\t-h Display this help\n");
  printf("\t-x Output in hexadecimal values\n");
  printf("\t-b Output in binary values\n");
  printf("\t-o <path> Save the output in file\n");
  printf("\n");
}

int main(int argc, char** argv)
{
  // Initialize the program state
  program_state* state = (program_state*) malloc(sizeof(program_state));
  state->input_path = NULL;
  state->output_path = NULL;
  state->is_hex = 1; // By default, it is hexadecimal
  
  // Iterate through each argument
  for (int i = 1; i < argc; i++)
    {
    if (argv[i][0] == '-')
    {
      switch(argv[i][1])
      {
        case 'h':
          print_help();
          return 0;
        break;
        case 'o':
          if (i == argc-1)
          {
            fprintf(stderr, "ERROR: Output path not defined.\n");
            return 1;
          }
          state->output_path = argv[i+1];
          i++;
        break;
        case 'x':
          state->is_hex = 1;
        break;
        case 'b':
          state->is_hex = 0;
        break;
        default:
          fprintf(stderr, "ERROR: Unknown argument %s.\n", argv[i]);
          return 1;
        break;
      }
    } else {
      state->input_path = argv[i];
    }
  }
  
  if (state->input_path == NULL)
  {
    fprintf(stderr, "ERROR: Input file not defined.\n");
  } else {
    hash_file(state);
  }

  free(state);
  return EXIT_SUCCESS;
}

