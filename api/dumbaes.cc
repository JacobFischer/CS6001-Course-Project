/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
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

#include "dumbaes.h"

#include "src/dumbaes.h"
#include <cstdlib>
#include <cstring>
#include <memory>



using namespace dumbaes;

unsigned char* dumbaes_128_encrypt_block(const unsigned char* plaintext,
                                         const unsigned char* key)
{
    printf("%s\n",plaintext);
    Block datablock;
    Key keyblock;

    std::memcpy(datablock.data(), plaintext, 16);
    std::memcpy(keyblock.data(), key, 16);

    Block ciphertext = encrypt_block(datablock, keyblock);

    unsigned char* result = static_cast<unsigned char*>(std::malloc(16));
    std::memcpy(result, ciphertext.data(), 16);
    return result;
}

unsigned char* dumbaes_128_decrypt_block(const unsigned char* ciphertext,
                                         const unsigned char* key)
{
    Block datablock;
    Key keyblock;

    std::memcpy(datablock.data(), ciphertext, 16);
    std::memcpy(keyblock.data(), key, 16);

    Block plaintext = decrypt_block(datablock, keyblock);

    unsigned char* result = static_cast<unsigned char*>(std::malloc(16));
    std::memcpy(result, plaintext.data(), 16);
    return result;
}

//#if 0
// TODO: Note these functions should just be C API wrappers around C++
// functions implemented in the src/ directory.

unsigned char* dumbaes_128_encrypt_cbc(const unsigned char* plaintext,
                                       size_t length,
                                       const unsigned char* key)//,
                                       //const unsigned char* iv)
{
    // TODO
}

unsigned char* dumbaes_128_decrypt_cbc(const  char* ciphertext,
                                       size_t length,
                                       const unsigned char* key)//,
                                       //const unsigned char* iv)
{

}

unsigned char* dumbaes_128_encrypt_ecb(const unsigned char* plaintext,
                                       size_t length,
                                       const unsigned char* key)
{
    Key keyblock;
    //printf("Orig: %s\n",plaintext);
    std::memcpy(keyblock.data(), key, 16);

    int num_blocks = length / 16 + 1;
    //num_blocks += ((length%16) ? 1:0);
    //printf("%d\n",num_blocks);
    
    //Block new_plaintext[num_blocks];
    //Block ciphertext[num_blocks];
    std::unique_ptr<Block> plaintext_array[num_blocks];
    std::unique_ptr<Block> ciphertext_array[num_blocks];
    
    /*
    std::unique_ptr<Block* []> plaintext_array[num_blocks];//(new Block*[num_blocks]);
    for(int block=0; block < num_blocks-1; block++)
    {
        
        std::memcpy(plaintext_array[block]->data(), plaintext+(16*block), 16);
        printf("Blocksmp: %s\n", plaintext_array[block]->data());
    }
    */
    /*
    Block **plaintext_array;
    plaintext_array = new Block *[num_blocks];
    plaintext_array[0] = new Block;
    */
    //std::unique_ptr< unique_ptr<Block[]> > plaintext_array(new uni
    
    /*
    std::unique_ptr<Block> test_ptr[2];//(new Block);
    test_ptr[0].reset(new Block);
    test_ptr[1].reset(new Block);
    std::memcpy(test_ptr[0]->data(), plaintext, 16);
    std::memcpy(test_ptr[1]->data(), plaintext+16, 16);
    printf("Ptr_test: %s\n", test_ptr[0]->data());
    printf("Ptr_test: %s\n", test_ptr[1]->data());
    */
    for(int block=0; block < num_blocks-1; block++)
    {
        //printf("Block: %d\n", block);
        plaintext_array[block].reset(new Block);
        std::memcpy(plaintext_array[block]->data(), plaintext+(16*block), 16);
        //std::memcpy(new_plaintext[block].data(), plaintext+(16*block), 16);
        //printf("Result: %s\n",new_plaintext[block].data());
    }
    plaintext_array[num_blocks-1].reset(new Block);
    std::memcpy(plaintext_array[num_blocks-1]->data(),
               (plaintext+(16*(num_blocks-1))), (length-(16*(num_blocks-1))));
    //new_plaintext[num_blocks-1].fill('0');// = "0000000000000000";
    /*
    printf("Block: %d\n", num_blocks-1);
    printf("Result: %s\n",new_plaintext[num_blocks-1].data());
    printf("Result: %s\n",new_plaintext[0].data());
    */
    //std::memcpy(new_plaintext[0].data(), plaintext+(16*0), 16);
    //new_plaintext[num_blocks-1].fill('0');
    //printf("Grab: %d\n",length-(16*(num_blocks-1)));
    //printf("At: %d\n", (16*(num_blocks-1)));
    //std::memcpy(new_plaintext[num_blocks-1].data(), (plaintext+(16*(num_blocks-1))),
    //           (length-(16*(num_blocks-1))));
    //printf("Result: %s\n",new_plaintext[num_blocks-1].data());
    //printf("done\n");
    //encrypt_ecb(new_plaintext, num_blocks, keyblock, ciphertext);
    
    encrypt_ecb(plaintext_array, length, keyblock, ciphertext_array);
    
    //unsigned char* result;// = "";
    
    unsigned char* result = static_cast<unsigned char*>(std::malloc(16*num_blocks));
    
    for(int block=0; block < num_blocks; block++)
    {
        //printf("Cipher: %s\n", ciphertext[block].data());
        std::memcpy((result+(16*block)), ciphertext_array[block]->data(), 16);
    }
    
    //std::memcpy(result, ciphertext, 16*num_blocks);
    return result;

    /*
    std::memcpy(datablock.data(), plaintext, 16);
    std::memcpy(keyblock.data(), key, 16);

    Block ciphertext = encrypt_block(datablock, keyblock);

    unsigned char* result = static_cast<unsigned char*>(std::malloc(16));
    std::memcpy(result, ciphertext.data(), 16);
    return result;
    */
    
    
}

unsigned char* dumbaes_128_decrypt_ecb(const char* ciphertext,
                                       size_t* c_length,
                                       const unsigned char* key)
{
    Key keyblock;
    size_t length = *c_length;
    std::memcpy(keyblock.data(), key, 16);

    int num_blocks = length / 16;
    
    std::unique_ptr<Block> plaintext_array[num_blocks];
    std::unique_ptr<Block> ciphertext_array[num_blocks];
    
    for(int block=0; block < num_blocks; block++)
    {
        ciphertext_array[block].reset(new Block);
        std::memcpy(ciphertext_array[block]->data(), ciphertext+(16*block), 16);
    }
    /*
    ciphertext_array[num_blocks-1].reset(new Block);
    std::memcpy(plaintext_array[num_blocks-1]->data(),
               (plaintext+(16*(num_blocks-1))), (length-(16*(num_blocks-1))));
    */           
    decrypt_ecb(ciphertext_array, length, keyblock, plaintext_array);           
               
    unsigned char* result = static_cast<unsigned char*>(std::malloc(16*num_blocks));

    for(int block=0; block < num_blocks; block++)
    {
        //printf("Cipher: %s\n", ciphertext[block].data());
        std::memcpy((result+(16*block)), plaintext_array[block]->data(), 16);
    }
    
    
    *c_length = length;
    //OLD
    /*
    Block new_ciphertext[num_blocks];
    Block plaintext[num_blocks];
    for(int block=0; block < num_blocks-1; block++)
    {
        std::memcpy(new_ciphertext[block].data(), (ciphertext+(16*block)), 16);
    }
    new_ciphertext[num_blocks-1].fill('0');
    std::memcpy(new_ciphertext[num_blocks-1].data(), (ciphertext+(16*(num_blocks-1))),           (length-(16*(num_blocks-1))));
    
    decrypt_ecb(new_ciphertext, num_blocks, keyblock, plaintext);
    
    //unsigned char* result;// = "";
    
    
    for(int block=0; block < num_blocks; block++)
    {
        std::memcpy((result+(16*block)), plaintext[block].data(), 16);
    }
    */
    return result;
}
//#endif
