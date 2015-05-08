/***********************************************************************
**
** Implementation of the internal Skein crypto functions.
**
** Source code author: Ergun Coruh, 2015.
**
** This algorithm and source code is released to the public domain.
************************************************************************/

#include <memory.h>
#include "skeincrypto.h"

void sk_init(SKCipher* ctx, const unsigned char* key, size_t bitSize)
{
	ctx->size = bitSize;
	memcpy(ctx->key, key, (bitSize / 8));
	memset(ctx->iv, 0, (bitSize / 8));
}

void sk_init_vec(SKCipher* ctx, const unsigned char* key, const unsigned char* vec, size_t bitSize)
{
	ctx->size = bitSize;
	memcpy(ctx->key, key, (bitSize / 8));
	memcpy(ctx->iv, vec, (bitSize / 8));
}

size_t sk_encrypt(SKCipher* ctx, char* inPtr, size_t inSize, char* outPtr, size_t outSize)
{
	size_t i, j;
	
	for (i = 0, j = 0; i < inSize && j < outSize; i++, j++)
	{
		if (i % (ctx->size / 8) == 0)
		{
			Skein_512_Init(&ctx->ctx, ctx->size);
			Skein_512_Update(&ctx->ctx, ctx->key, (ctx->size / 8));
			Skein_512_Update(&ctx->ctx, ctx->iv, (ctx->size / 8));
			Skein_512_Final(&ctx->ctx, ctx->key);
		}
		outPtr[j] = inPtr[i] ^ ctx->key[i % (ctx->size / 8)];
		ctx->iv[i % (ctx->size / 8)] = outPtr[j];
	}
	return j;
}

size_t sk_decrypt(SKCipher* ctx, char* inPtr, size_t inSize, char* outPtr, size_t outSize)
{
	size_t i, j;
	
	for (i = 0, j = 0; i < inSize && j < outSize; i++, j++)
	{
		if (i % (ctx->size / 8) == 0)
		{
			Skein_512_Init(&ctx->ctx, ctx->size);
			Skein_512_Update(&ctx->ctx, ctx->key, (ctx->size / 8));
			Skein_512_Update(&ctx->ctx, ctx->iv, (ctx->size / 8));
			Skein_512_Final(&ctx->ctx, ctx->key);
		}
		outPtr[j] = inPtr[i] ^ ctx->key[i % (ctx->size / 8)];
		ctx->iv[i % (ctx->size / 8)] = inPtr[i];
	}
	return j;
}

unsigned char* sk_get_key(const char* password, int bitSize)
{
	Skein_512_Ctxt_t  ctx;
   static unsigned char key[SKEIN_BYTES];
   Skein_512_Init(&ctx, bitSize);
	Skein_512_Update(&ctx, (unsigned char*)password, strlen(password));
   Skein_512_Final(&ctx, key);
	return key;
}

int sk_verify_key(const char* password, unsigned char* hash, int bitSize)
{
   Skein_512_Ctxt_t  ctx;
   unsigned char key[SKEIN_BYTES];
   Skein_512_Init(&ctx, bitSize);
   Skein_512_Update(&ctx, (unsigned char*)password, strlen(password));
   Skein_512_Final(&ctx, key);
   return (memcmp(key,hash,SKEIN_BYTES));
}



