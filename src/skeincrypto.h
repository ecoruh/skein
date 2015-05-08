/***********************************************************************
**
** Implementation of the internal Skein crypto functions.
**
** Source code author: Ergun Coruh, 2015.
**
** This algorithm and source code is released to the public domain.
************************************************************************/

#ifndef SKEINCRYPTO_H
#define SKEINCRYPTO_H

#include <wchar.h>
#include <wctype.h>
#include "skein.h"

#define SKEIN_BITS  512             // encryption bit size
#define SKEIN_BYTES SKEIN_BITS/8    // encryption byte size

typedef struct 
{
    Skein_512_Ctxt_t  ctx;
    size_t            size;
    unsigned char     key[SKEIN_BYTES];
    unsigned char     iv[SKEIN_BYTES];
} SKCipher;

/**
 \brief Initialise cipher with a key. A key is a password hash.
 \note  Use getKey to calculate key from a password.
 */
void sk_init(SKCipher* ctx, const unsigned char* key, size_t bitSize);

/**
  \brief Initialise cipher with a key and a vector. A key is a password hash.
  \note  Use getKey to calculate key from a password.
  */
void sk_init_vec(SKCipher* ctx, const unsigned char* key, const unsigned char* vec, size_t bitSize);

/**
  \brief Encrypt array of bytes using a pre-initialised cipher

  \note  Each encrypt() must be preceded by a cipherInit().
  \note  Both buffers must be allocated by the caller.
  \note  Normally outSize should be equal to inSize.

  \param[in]    ctx         A cipher context
  \param[in]    inPtr       Pointer to input plain data stream
  \param[in]    inSize      Number of bytes in plain data stream inPtr
  \param[out]   outPtr      Pointer to output encrypted stream
  \param[in]    outSize     Number of bytes in encrypted stream outPtr
  */
size_t sk_encrypt(SKCipher* ctx, char* inPtr, size_t inSize, char* outPtr, size_t outSize);

/**
  \brief Decrypt array of bytes using a pre-initialised cipher

  \note  Each decrypt() must be preceded by a cipherInit().
  \note  Both buffers must be allocated by the caller.
  \note  Normally outSize should be equal to inSize.

  \param[in]    ctx         A cipher context
  \param[in]    inPtr       Pointer to input encrypted stream
  \param[in]    inSize      Number of bytes in encrypted stream inPtr
  \param[out]   outPtr      Pointer to output decrypted (plain) stream
  \param[in]    outSize     Number of bytes in decrypted stream outPtr
  */
size_t sk_decrypt(SKCipher* ctx, char* inPtr, size_t inSize, char* outPtr, size_t outSize);

/**
  \brief Calculate a key from a password. A key is simply a hash value
         of a password.
  \param[in]    password    A password string
  \param[in]    bitSize     bit-size
  */
unsigned char* sk_get_key(const char* password, int bitSize);

/**
  \brief Verify a password against a key (hash).
  \param[in]    password    A password string
  \param[in]    hash        Hash to compare
  \param[in]    bitSize     bit-size
  */
int sk_verify_key(const char* password, unsigned char* hash, int bitSize);


#endif // SKEINCRYPTO_H_
