#ifndef CIPHER_H_HEADER_GUARD
#define CIPHER_H_HEADER_GUARD

#include <stdint.h> /* uint8_t, uint64_t */
#include <stdlib.h> /* malloc */

#include <gcrypt.h>

#include "common.h"

#define IV_LEN      16
#define MAX_KEY_LEN 32

uint8_t *Encrypt(uint8_t *msg,    size_t size, char *key, int keyLen, uint8_t *iv);
uint8_t *Decrypt(uint8_t *cipher, size_t size, char *key, int keyLen, uint8_t *iv);

#endif
