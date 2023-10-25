#ifndef TIGER_H
#define TIGER_H

#include "sboxes.c"
#include "tiger.c"

// Function returns back hashed output
byte* tiger(byte* plain, word64 plain_length, word64* length);

#endif
