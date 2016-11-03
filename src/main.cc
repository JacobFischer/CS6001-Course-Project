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

#include "polynomial.h"
#include <cstdint>
#include <cstdio>

using namespace dumbaes;

/*
 * To make life simple, we're not yet going to bother with silly things like
 * "user input." Put your input here and recompile the file.
 */
int main() {
    // Homework 3 problem 2
    // Default characteristic and IP are large enough to avoid mod operations.
    {
        Polynomial p1{0b101111};
        Polynomial p2{0b111};
        std::printf("Homework 3 problem 2\n");
        std::printf("%x + %x = %x\n", p1.value(), p2.value(), (p1 + p2).value());
        std::printf("%x - %x = %x\n", p1.value(), p2.value(), (p1 - p2).value());
        std::printf("%x * %x = %x\n", p1.value(), p2.value(), (p1 * p2).value());
        std::printf("%x / %x = %x\n", p1.value(), p2.value(), (p1 / p2).value());
        std::printf("\n");
    }

    // Homework 3 problem 3
    {
        uint16_t ip1 = 0b1011;
        uint16_t ip2 = 0b1101;
        Polynomial p1{0b00000010, ip1, 3};
        Polynomial r1 = multiplicative_inverse(p1);
        Polynomial p2{0b00000010, ip2, 3};
        Polynomial r2 = multiplicative_inverse(p2);
        std::printf("Homework 3 problem 3\n");
        std::printf("Multiplicative inverse of %x with IP %x is %x\n", p1.value(), ip1, r1.value());
        std::printf("Multiplicative inverse of %x with IP %x is %x\n", p2.value(), ip2, r2.value());
        std::printf("\n");
    }

    // Homework 3 problem 4
    {
        Polynomial p1{0b111, 0b1011, 3};
        Polynomial p2{0b101, 0b1011, 3};
        Polynomial p3{0b111, 0b1101, 3};
        Polynomial p4{0b101, 0b1101, 3};

        std::printf("Homework 3 problem 4\n");
        std::printf("With irreducible polynomial 1011:\n");
        std::printf("%x + %x = %x\n", p1.value(), p2.value(), (p1 + p2).value());
        std::printf("%x - %x = %x\n", p1.value(), p2.value(), (p1 - p2).value());
        std::printf("%x * %x = %x\n", p1.value(), p2.value(), (p1 * p2).value());
        std::printf("%x / %x = %x\n", p1.value(), p2.value(), (p1 / p2).value());
        std::printf("With irreducible polynomial 1101:\n");
        std::printf("%x + %x = %x\n", p3.value(), p4.value(), (p3 + p4).value());
        std::printf("%x - %x = %x\n", p3.value(), p4.value(), (p3 - p4).value());
        std::printf("%x * %x = %x\n", p3.value(), p4.value(), (p3 * p4).value());
        std::printf("%x / %x = %x\n", p3.value(), p4.value(), (p3 / p4).value());
        std::printf("\n");
    }

    // Homework 3 problem 5
    {
        Polynomial p1{0b10101101};
        Polynomial p2{0b1101};
        std::printf("Homework 3 problem 5\n");
        std::printf("Multiplicative inverse of %x is %x\n", p1.value(), multiplicative_inverse(p1).value());
        std::printf("Multiplicative inverse of %x is %x\n", p2.value(), multiplicative_inverse(p2).value());
        std::printf("\n");
    }
}
