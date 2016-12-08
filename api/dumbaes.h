/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (c) 2016 Michael Catanzaro <michael.catanzaro@mst.edu>
 * Copyright (c) 2016 Jacob Fischer <jtf3m8@mst.edu>
 * Copyright (c) 2016 Christian Storer <cs9yb@mst.edu>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef DUMBAES_H
#define DUMBAES_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * dumbaes_128_encrypt_block:
 * @plaintext: the plaintext to encrypt
 * @key: the private key to use for the encryption
 *
 * Encrypts a 128-bit (16-byte) block of data @plaintext using the
 * private key @key. Both @plaintext and @key must be exactly 16 bytes
 * long and need not be %NULL-terminated.
 *
 * Returns: 128-bit ciphertext, not %NULL-terminated. Free with free().
 */
unsigned char *dumbaes_128_encrypt_block (const unsigned char *plaintext,
                                          const unsigned char *key);

/**
 * dumbaes_128_decrypt_block:
 * @ciphertext: the ciphertext to decrypt
 * @key: the private key to use for the decryption
 *
 * Decrypts a 128-bit (16-byte) block of data @ciphertext using the
 * private key @key. Both @ciphertext and @key must be exactly 16 bytes
 * long and need not be %NULL-terminated.
 *
 * Returns: 128-bit plaintext, not %NULL-terminated. Free with free().
 */
unsigned char *dumbaes_128_decrypt_block (const unsigned char *ciphertext,
                                          const unsigned char *key);

//#if 0
// TODO: Implement these functions

/**
 * dumbaes_128_encrypt_cbc:
 * @plaintext: the plaintext to encrypt
 * @length: pointer length of the plaintext to encrypt, in bytes
 * @key: the private key to use for the encryption
 * @iv: the initialization vector to use for the encryption
 *
 * Encrypts a @plaintext of length @length using the private key @key.
 * Both @key and @iv must be exactly 16 bytes long and need not be
 * %NULL-terminated. The data will be encrypted using cipher block
 * chaining. Note that @plaintext, @key, and @iv may all contain
 * embedded %NULL characters.
 *
 * Returns: ciphertext of length @length. Free with free().
 */
unsigned char *dumbaes_128_encrypt_cbc (const unsigned char *plaintext,
                                        size_t              *length,
                                        const unsigned char *key,
                                        const unsigned char *iv);

/**
 * dumbaes_128_decrypt_cbc:
 * @ciphertext: the ciphertext to decrypt
 * @length: pointer length of the ciphertext to decrypt, in bytes
 * @key: the private key to use for the decryption
 * @iv: the initialization vector to use for the decryption
 *
 * Decrypts a @ciphertext encrypted by dumbaes_128_encrypt_cbc() of
 * length @length using the private key @key. Both @key and @iv must be
 * exactly 16 bytes long and need not be %NULL-terminated. Note that
 * @plaintext, @key, and @iv may all contain embedded %NULL characters.
 *
 * Returns: plaintext of length @length. Free with free().
 */
unsigned char *dumbaes_128_decrypt_cbc (const unsigned char *ciphertext,
                                        size_t              *length,
                                        const unsigned char *key,
                                        const unsigned char *iv);

/**
 * dumbaes_128_encrypt_ecb:
 * @plaintext: the plaintext to encrypt
 * @length: pointer to length of the plaintext to encrypt, in bytes
 * @key: the private key to use for the encryption
 *
 * Encrypts a @plaintext of length @length using the private key @key.
 * @key must be exactly 16 bytes long and need not be %NULL-terminated.
 * The data will be encrypted using electronic codebook mode. Note that
 * both @plaintext and @key may contain embedded %NULL characters. Also
 * note that this function is insecure and should not be used with data
 * that is more than 16 bytes long. Use dumbaes_128_encrypt_cbc()
 * instead.
 *
 * Returns: ciphertext of length @length. Free with free().
 */
unsigned char *dumbaes_128_encrypt_ecb (const unsigned char *plaintext,
                                        size_t              *length,
                                        const unsigned char *key);

/**
 * dumbaes_128_decrypt_ecb:
 * @ciphertext: the ciphertext to decrypt
 * @length: pointer to length of the ciphertext to decrypt, in bytes
 * @key: the private key to use for the decryption
 *
 * Decrypts a @ciphertext encrypted by dumbaes_128_encrypt_ecb() of
 * length @length using the private key @key. @key must be exactly 16
 * bytes long and need not be %NULL-terminated. Note that both
 * @plaintext and @key may contain embedded %NULL characters.
 *
 * Returns: plaintext of length @length. Free with free().
 */
unsigned char *dumbaes_128_decrypt_ecb (const unsigned char *ciphertext,
                                        size_t              *length,
                                        const unsigned char *key);

unsigned char *dumbaes_128_generate_iv (void);
                                        
                                        
                                        
#ifdef __cplusplus
} // extern "C"
#endif

#endif // DUMBAES_H
