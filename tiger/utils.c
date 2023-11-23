#ifndef UTILS_C
#define UTILS_C

/**
 *  Appends the character 'ch' to an array of 'char' data types,
 *  and returns the array pointer.
 */
byte* append_to_chars(byte* array, byte ch, word64* length)
{
  word64 l = (*length) + 1;
  array = (byte*) realloc(array, sizeof(byte)*(l+1));
  array[l-1] = ch;
  *length = l;
  return array;
}

/**
 *  Print an array of bytes in hexadecimal value.
 */
void printxs(byte* array, word64 length)
{
  for(word64 i = 0; i < length; i++)
  {
    printf("%02X", array[i]);
  }
  printf("\n");
}

/**
 *  Print an array of bytes in binary value.
 */
void printbs(byte* array, word64 length)
{
  for(word64 i = 0; i < length; i++)
  {
    printf("%c", array[i]);
  }
  printf("\n");
}

/**
 *  Appends a byte (unsigned char, 8 bits) into a variable of 8 bytes (64 bits)
 *  and returns it.
 */
word64 append_byte(word64 n, byte ch) {
    return ((n << BYTE_SIZE) | ch);
}

/**
 *  This function extracts a certain byte of a 'long long unsigned int'
 *  using the index of the byte.
 */
byte extract_byte(word64 n, int index)
{
  return (( n ) >> ( BYTE_SIZE * index )) & 0xFF;
}

/**
 *  Input until EOF is put into an 'unsigned char' array
 *  to be returned, with the length being altered.
 */
byte* get_eof_input(word64* length)
{
  // Initialize the array to be filled
  word64 l = 0;
  byte* array = (byte*) malloc(sizeof(byte)*(l+1));
  
  // Read input until End Of File 'EOF' or until '\0'
  word64 ch;
  while ( (ch = (word64)getchar()) != MASK) // compare input to EOF(0xFFFFFFFFFFFFFFFF), allows us to input '0xFF'
  {
    array = append_to_chars(array, (byte)ch, &l); // Append inputted character
  }
  *length = l;    // Update the value in the pointer of 'length'
 
  return array;
}

/**
 *  Write an array of bytes into a file.
 *    is_hex : Writes in hex mode if it is 1
 */
void write_to_file(byte* filename, byte* array, word64 array_length, int is_hex)
{
  FILE* file;
  file = fopen(filename, "w");

  // Check if the file was successfully created
  if (file == NULL)
  {
    fprintf(stderr, "ERROR: The file '%s' could not be created.\n", filename);
    return;
  }
  
  // Fill in the file
  for(word64 i = 0; i < array_length; i++)
  {
    if (is_hex)
    {
      fprintf(file, "%02X", array[i]);
    } else {
      fprintf(file, "%c", array[i]);
    }
  }

  // Close the file
  fclose(file);
  return;
}

/**
 *  Read the contents of a file and put them into an
 *  array of bytes.
 */
byte* read_file_bytes(char* filename, word64* length)
{

  FILE* file;
  file = fopen(filename, "r");

  if (file == NULL)
  {
    fprintf(stderr, "ERROR: The file '%s' does not exist.\n", filename);
    return NULL;
  }

  // Initialize the array to be filled
  word64 l = 0;
  byte* array = (byte*) malloc(sizeof(byte)*(l+1));
  
  // Read input until End Of File 'EOF' or until '\0'
  word64 ch;
  while ( (ch = (word64)fgetc(file)) != MASK) // compare input to EOF(0xFFFFFFFFFFFFFFFF), allows us to input '0xFF'
  {
    array = append_to_chars(array, (byte)ch, &l); // Append inputted character
  }
  fclose(file);
  *length = l;    // Update the value in the pointer of 'length'
  
  return array;
}


/**
 *  Converts 3 'word64' values
 *  into an array of bytes.
 */
byte* word64_to_array(
  word64 a,
  word64 b,
  word64 c,
  word64* length
) {

  // Initialize the array
  word64 l = BYTE_SIZE*3;
  byte* array = (byte*)malloc(sizeof(byte)*(l+1));
  // Fill the array with the extracted
  // bytes from the 3 'long long unsigned int's
  for (int i = 0; i < BYTE_SIZE; i++)
  {
    array[i] = extract_byte(a, BYTE_SIZE-1-i);
  }
  for (int i = 0; i < BYTE_SIZE; i++)
  {
    array[i+BYTE_SIZE] = extract_byte(b, BYTE_SIZE-1-i);
  }
  for (int i = 0; i < BYTE_SIZE; i++)
  {
    array[i+BYTE_SIZE*2] = extract_byte(c, BYTE_SIZE-1-i);
  }

  // Override the pointers value
  *length = l;
  return array;
}

/**
 *  Divide an array of bytes (length should be multiple of 64 bytes)
 *  into 8 chunks of 64bit words.
 */
void divide_64_bytes(
  word64 i, // Index to apply
  byte* array,
  word64 length,
  word64* x0_,
  word64* x1_,
  word64* x2_,
  word64* x3_,
  word64* x4_,
  word64* x5_,
  word64* x6_,
  word64* x7_
) {
  // Create local variables to apply calculations to
  word64 x0, x1, x2, x3, x4, x5, x6, x7;
  x0 = *x0_;
  x1 = *x1_;
  x2 = *x2_;
  x3 = *x3_;
  x4 = *x4_;
  x5 = *x5_;
  x6 = *x6_;
  x7 = *x7_;

  // Fill the values of each 8 byte word using the 'padded' array
  for (word64 j = 0; j < BYTE_SIZE; j++)
  {
    x0 = append_byte(x0, array[i + j]); // 0 -> 8
    x1 = append_byte(x1, array[i + j + BYTE_SIZE*1]); // 8 -> 16
    x2 = append_byte(x2, array[i + j + BYTE_SIZE*2]); // 16 -> 24
    x3 = append_byte(x3, array[i + j + BYTE_SIZE*3]); // 24 -> 32
    x4 = append_byte(x4, array[i + j + BYTE_SIZE*4]); // 32 -> 40
    x5 = append_byte(x5, array[i + j + BYTE_SIZE*5]); // 40 -> 48
    x6 = append_byte(x6, array[i + j + BYTE_SIZE*6]); // 48 -> 56
    x7 = append_byte(x7, array[i + j + BYTE_SIZE*7]); // 56 -> 64
  }

  // Update the pointers with the correct values
  *x0_ = x0;
  *x1_ = x1;
  *x2_ = x2;
  *x3_ = x3;
  *x4_ = x4;
  *x5_ = x5;
  *x6_ = x6;
  *x7_ = x7;
}



#endif
