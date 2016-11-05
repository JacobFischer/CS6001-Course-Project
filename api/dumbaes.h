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

#ifndef __cplusplus
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

#if 0
// TODO: Implement these functions

/**
 * dumbaes_128_encrypt_cbc:
 * @plaintext: the %NULL-terminated plaintext to encrypt
 * @key: the private key to use for the encryption
 *
 * Encrypts an arbitrary-length %NULL-terminated @plaintext using the
 * private key @key. @key must be exactly 16 bytes long and need not be
 * %NULL-terminated. The data will be encrypted using cipher block
 * chaining.
 *
 * Returns: %NULL-terminated ciphertext. Free with free().
 */
char *dumbaes_128_encrypt_cbc (const char          *plaintext,
                               const unsigned char *key);

/**
 * dumbaes_128_decrypt_cbc:
 * @ciphertext: the %NULL-terminated ciphertext to decrypt
 * @key: the private key to use for the decryption
 *
 * Decrypts an arbitrary-length %NULL-terminated @ciphertext using the
 * private key @key. @key must be exactly 16 bytes long and need not be
 * %NULL-terminated. The data will be decrypted using cipher block
 * chaining.
 *
 * Returns: %NULL-terminated plaintext. Free with free().
 */
char *dumbaes_128_encrypt_cbc (const char          *ciphertext,
                               const unsigned char *key);

#endif

#ifndef __cplusplus
} // extern "C"
#endif

#endif // DUMBAES_H