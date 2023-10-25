/**
 *  This function applies padding to an 'unsigned char' array
 *  and returns it, also overriding the 'new_length' variable with the
 *  updated arrays length.
 */
byte* tiger_padding(byte* array, word64 length, word64* new_length)
{  
  // Store the multiple of 8 for the original length
  word64 multiple_8 = length * 8;

  // First, we append One Byte (0x01) into the array 
  word64 l = length;
  array = append_to_chars(array, ONE_BYTE, &l);
 
  // Second, we append Zero Byte (0x00) until the (Length % 64) equals 56.
  while ( l % 64 != 56 )
  {
    array = append_to_chars(array, ZERO_BYTE, &l);
  }

  // Third, we append the bytes of 'multiple_8' at the end, 8 times
  for (int i = 0 ; i < BYTE_SIZE; i++)
  {
    array = append_to_chars(array, extract_byte(multiple_8, BYTE_SIZE-1-i), &l);
  }

  // Override the 'new_length' pointers value
  *new_length = l;
  return array;
}

/**
 *  The 'round' function used for the Tiger Hashing algorithm.
 */
void tiger_round( 
    word64* a_, 
    word64* b_, 
    word64* c_, 
    word64 x,
    word64 mul
) {
  // We create local variables to update
  // the sent pointers
  word64 a, b, c;
  a = *a_;
  b = *b_;
  c = *c_;

  // Second, the values 'a', 'b', 'c' will be updated according to
  // found values
  c ^= x;
  //a -= t1[c_0] ^ t2[c_2] ^ t3[c_4] ^ t4[c_6];
  a -=  t1[ extract_byte(c,  0) ] ^ t2[ extract_byte(c,  2) ] 
      ^ t3[ extract_byte(c,  4) ] ^ t4[ extract_byte(c,  6) ];
  //b += t4[c_1] ^ t3[c_3] & t2[c_5] ^ t1[c_7];
  b +=  t4[ extract_byte(c,  1) ] ^ t3[ extract_byte(c,  3) ] 
      ^ t2[ extract_byte(c,  5) ] ^ t1[ extract_byte(c,  7) ];
  b *= mul;

  // Finally, we update the values inside the pointer
  // with the locally calculated values
  *a_ = a;
  *b_ = b;
  *c_ = c;
}

/**
 *  The 'pass' function used in the Tiger Hashing algorithm.
 */
void pass(
    word64 mul,
    word64* a_,
    word64* b_,
    word64* c_,
    word64 x0,
    word64 x1,
    word64 x2,
    word64 x3,
    word64 x4,
    word64 x5,
    word64 x6,
    word64 x7
) {
  // Create local variables for the calculations
  word64 a, b, c;
  a = *a_;
  b = *b_;
  c = *c_;

  // Execute the calculations
  tiger_round(&a, &b, &c, x0, mul);
  tiger_round(&b, &c, &a, x1, mul);
  tiger_round(&c, &a, &b, x2, mul);
  tiger_round(&a, &b, &c, x3, mul);
  tiger_round(&b, &c, &a, x4, mul);
  tiger_round(&c, &a, &b, x5, mul);
  tiger_round(&a, &b, &c, x6, mul);
  tiger_round(&b, &c, &a, x7, mul);

  // Update the pointers with the correct values
  *a_ = a;
  *b_ = b;
  *c_ = c;
}

/**
 *  The 'keyschedule' function used for the Tiger Algorithm.
 */
void key_schedule (
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

  // Execute the calculations
  x0 -= x7 ^ 0xA5A5A5A5A5A5A5A5LL;
  x1 ^= x0;
  x2 += x1;
  x3 -= x2 ^ ((~x1)<<19);
  x4 ^= x3;
  x5 += x4;
  x6 -= x5 ^ ((~x4)>>23);
  x7 ^= x6;
  x0 += x7;
  x1 -= x0 ^ ((~x7)<<19);
  x2 ^= x1;
  x3 += x2;
  x4 -= x3 ^ ((~x2)>>23);
  x5 ^= x4;
  x6 += x5;
  x7 -= x6 ^ 0x0123456789ABCDEFLL;

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

/**
 *  The 'compress' function of the Tiger Hash Algorithm.
 */
void tiger_compress(
    word64* a_, 
    word64* b_, 
    word64* c_,
    word64 x0,
    word64 x1,
    word64 x2,
    word64 x3,
    word64 x4,
    word64 x5,
    word64 x6,
    word64 x7
) {
  // Create local variables
  // using the pointer values
  word64 a, b, c, tmp_a, aa, bb, cc;
  a = *a_;
  b = *b_;
  c = *c_;
 
  // save_abc
  aa = a;
  bb = b;
  cc = c;

  for (word64 j = 0; j < PASSES; j++)
  {
    if ( j != 0 ) 
    {
      key_schedule(&x0, &x1, &x2, &x3, &x4, &x5, &x6, &x7);
    }

    pass( ( j==0?5: j==1?7: 9 ) , &a, &b, &c, 
              x0, x1, x2, x3, x4, x5, x6, x7);

    tmp_a = a;
    a = c;
    c = b;
    b = tmp_a;
  }

  // Feed forward
  a ^= aa;
  b -= bb;
  c += cc;

  // Update the pointers with local values
  *a_ = a;
  *b_ = b;
  *c_ = c;
}


/**
 *  Plain text is used to compute a hash value.
 */
byte* tiger(byte* plain, word64 plain_length, word64* length)
{
  // The length of the 'padded' array is divisible by 64, so we can
  // divide it into chunks to be processed with ease.
  word64 padded_length;
  byte* padded = tiger_padding(plain, plain_length, &padded_length);
   
  // Initialize the 'a', 'b', 'c' values for the algorithm 
  word64 a,b,c;
  a = A;
  b = B;
  c = C;

  // Go through each X chunk, that is size of 64 bytes (512 bits)
  for (word64 i = 0; i < padded_length; i=i+64)
  {
    // For each 64 byte X chunk, divide it into chunks of 8 bytes
    // The data type 'long long unsigned int' can hold 8 bytes
    word64 x0, x1, x2, x3, x4, x5, x6, x7;
    x0 = ZERO_BYTE;
    x1 = ZERO_BYTE;
    x2 = ZERO_BYTE;
    x3 = ZERO_BYTE;
    x4 = ZERO_BYTE;
    x5 = ZERO_BYTE;
    x6 = ZERO_BYTE;
    x7 = ZERO_BYTE;

    // Fill the values from x0 -> x7 by dividing the padded array
    divide_64_bytes(i, padded, padded_length, &x0, &x1, &x2, &x3, &x4, &x5, &x6, &x7);

    // Compress for the chunk
    tiger_compress(&a, &b, &c, x0, x1, x2, x3, x4, x5, x6, x7);
  }

  // Create an array of bytes from the 'long long unsigned int' values
  word64 hash_length;
  byte* hash = word64_to_array(a, b, c, &hash_length);

  // Override the 'length' pointers value
  *length = hash_length;

  // Free the unnecessary values
  free(padded);
  return hash;
}

