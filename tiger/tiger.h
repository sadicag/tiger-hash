#ifndef TIGER_H
#define TIGER_H

/**
 *  Classifications according to C Data Types.
 */
typedef unsigned long long int word64;
typedef unsigned long word32;
typedef unsigned char byte;

/**
 *  Global Constants for the Tiger Hash Algorithm.
 */
#define A         0x0123456789ABCDEFLL // The initial 'a' register used for Tiger Hash
#define B         0xFEDCBA9876543210LL // The initial 'b' register used for Tiger Hash
#define C         0xF096A5B4C3B2E187LL // The initial 'c' register used for Tiger Hash
#define MASK      0xFFFFFFFFFFFFFFFFLL // The Mask to simulate unsigned operations
#define ONE_BYTE  0x01 // This variable holds one byte
#define ZERO_BYTE 0x00 // This variable holds zero byte
#define BYTE_SIZE 8 // Size of a Byte, also number of Bytes in 'long long unsigned int'
#define PASSES    3 // The number of passes of the hash function, must be at least three

#include "utils.c"
#include "sboxes.c"
#include "tiger.c"

// Function returns back hashed output
byte* tiger(byte* plain, word64 plain_length, word64* length);

#endif
