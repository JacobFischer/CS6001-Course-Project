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

#include "src/polynomial.h"
#include <glib.h>
#include <locale.h>

using namespace dumbaes;

// Test cases from https://en.wikipedia.org/wiki/Finite_field_arithmetic#Addition_and_subtraction
static void add_subtract1()
{
    Polynomial p1{0b1011};
    Polynomial p2{0b1100};

    g_assert_cmpuint((p1 + p2).value(), ==, 0b0111);
    g_assert_cmpuint((p1 - p2).value(), ==, 0b0111);
}

static void add_subtract2()
{
    Polynomial p1{0b00010100};
    Polynomial p2{0b01000100};

    g_assert_cmpuint((p1 + p2).value(), ==, 0b01010000);
    g_assert_cmpuint((p1 - p2).value(), ==, 0b01010000);
}

static void add_subtract3()
{
    Polynomial p1{0b011};
    Polynomial p2{0b101};

    g_assert_cmpuint((p1 + p2).value(), ==, 0b110);
    g_assert_cmpuint((p1 - p2).value(), ==, 0b110);
}

static void add_subtract4()
{
    Polynomial p1{0b1010};
    Polynomial p2{0b0101};
    Polynomial p3{0b1111};

    g_assert_cmpuint((p1 + p2).value(), ==, p3.value());
    g_assert_cmpuint((p1 - p2).value(), ==, p3.value());
}

static void add_subtract5()
{
    Polynomial p1{0b110};
    Polynomial p2{0b110};

    g_assert_cmpuint((p1 + p2).value(), ==, 0);
    g_assert_cmpuint((p1 - p2).value(), ==, 0);
}

static void multiply1()
{
    // Example in section 4.2 of FIPS 197.
    Polynomial p1{0x57};
    Polynomial p2{0x83};

    g_assert_cmpuint((p1 * p2).value(), ==, 0xc1);
}

static void multiply2()
{
    // Example in section 4.2.1 of FIPS 197.
    Polynomial p1{0x57};
    Polynomial p2{0x13};

    g_assert_cmpuint((p1 * p2).value(), ==, 0xfe);
}

static void multiply3()
{
    // https://en.wikipedia.org/wiki/Finite_field_arithmetic#Rijndael.27s_finite_field
    Polynomial p1{0x53};
    Polynomial p2{0xca};

    g_assert_cmpuint((p1 * p2).value(), ==, 0x01);
}

static void multiply4()
{
    // Stinson example 6.6
    uint8_t ip = 0b1011;
    Polynomial p1{0b101, ip, 3};
    Polynomial p2{0b111, ip, 3};

    g_assert_cmpuint((p1 * p2).value(), ==, 0b110);
}

static void multiply5()
{
    // Anything multiplied by zero had better be zero.
    Polynomial p0{0};
    Polynomial p1{0xca};
    Polynomial p2{0x01};
    Polynomial p3{0b1011};
    Polynomial p4{0b1100};

    g_assert_cmpuint((p0 * p1).value(), ==, 0);
    g_assert_cmpuint((p0 * p2).value(), ==, 0);
    g_assert_cmpuint((p0 * p3).value(), ==, 0);
    g_assert_cmpuint((p0 * p4).value(), ==, 0);
}

static void multiplication_table()
{
    uint8_t ip = 0b1011;

    // Full multiplication table for GF(2^3) from Stinson Example 6.6
    g_assert_cmpuint((Polynomial{0b001, ip, 3} * Polynomial{0b001, ip, 3}).value(), ==, 0b001);
    g_assert_cmpuint((Polynomial{0b001, ip, 3} * Polynomial{0b010, ip, 3}).value(), ==, 0b010);
    g_assert_cmpuint((Polynomial{0b001, ip, 3} * Polynomial{0b011, ip, 3}).value(), ==, 0b011);
    g_assert_cmpuint((Polynomial{0b001, ip, 3} * Polynomial{0b100, ip, 3}).value(), ==, 0b100);
    g_assert_cmpuint((Polynomial{0b001, ip, 3} * Polynomial{0b101, ip, 3}).value(), ==, 0b101);
    g_assert_cmpuint((Polynomial{0b001, ip, 3} * Polynomial{0b110, ip, 3}).value(), ==, 0b110);
    g_assert_cmpuint((Polynomial{0b001, ip, 3} * Polynomial{0b111, ip, 3}).value(), ==, 0b111);

    g_assert_cmpuint((Polynomial{0b010, ip, 3} * Polynomial{0b001, ip, 3}).value(), ==, 0b010);
    g_assert_cmpuint((Polynomial{0b010, ip, 3} * Polynomial{0b010, ip, 3}).value(), ==, 0b100);
    g_assert_cmpuint((Polynomial{0b010, ip, 3} * Polynomial{0b011, ip, 3}).value(), ==, 0b110);
    g_assert_cmpuint((Polynomial{0b010, ip, 3} * Polynomial{0b100, ip, 3}).value(), ==, 0b011);
    g_assert_cmpuint((Polynomial{0b010, ip, 3} * Polynomial{0b101, ip, 3}).value(), ==, 0b001);
    g_assert_cmpuint((Polynomial{0b010, ip, 3} * Polynomial{0b110, ip, 3}).value(), ==, 0b111);
    g_assert_cmpuint((Polynomial{0b010, ip, 3} * Polynomial{0b111, ip, 3}).value(), ==, 0b101);

    g_assert_cmpuint((Polynomial{0b011, ip, 3} * Polynomial{0b001, ip, 3}).value(), ==, 0b011);
    g_assert_cmpuint((Polynomial{0b011, ip, 3} * Polynomial{0b010, ip, 3}).value(), ==, 0b110);
    g_assert_cmpuint((Polynomial{0b011, ip, 3} * Polynomial{0b011, ip, 3}).value(), ==, 0b101);
    g_assert_cmpuint((Polynomial{0b011, ip, 3} * Polynomial{0b100, ip, 3}).value(), ==, 0b111);
    g_assert_cmpuint((Polynomial{0b011, ip, 3} * Polynomial{0b101, ip, 3}).value(), ==, 0b100);
    g_assert_cmpuint((Polynomial{0b011, ip, 3} * Polynomial{0b110, ip, 3}).value(), ==, 0b001);
    g_assert_cmpuint((Polynomial{0b011, ip, 3} * Polynomial{0b111, ip, 3}).value(), ==, 0b010);

    g_assert_cmpuint((Polynomial{0b100, ip, 3} * Polynomial{0b001, ip, 3}).value(), ==, 0b100);
    g_assert_cmpuint((Polynomial{0b100, ip, 3} * Polynomial{0b010, ip, 3}).value(), ==, 0b011);
    g_assert_cmpuint((Polynomial{0b100, ip, 3} * Polynomial{0b011, ip, 3}).value(), ==, 0b111);
    g_assert_cmpuint((Polynomial{0b100, ip, 3} * Polynomial{0b100, ip, 3}).value(), ==, 0b110);
    g_assert_cmpuint((Polynomial{0b100, ip, 3} * Polynomial{0b101, ip, 3}).value(), ==, 0b010);
    g_assert_cmpuint((Polynomial{0b100, ip, 3} * Polynomial{0b110, ip, 3}).value(), ==, 0b101);
    g_assert_cmpuint((Polynomial{0b100, ip, 3} * Polynomial{0b111, ip, 3}).value(), ==, 0b001);

    g_assert_cmpuint((Polynomial{0b101, ip, 3} * Polynomial{0b001, ip, 3}).value(), ==, 0b101);
    g_assert_cmpuint((Polynomial{0b101, ip, 3} * Polynomial{0b010, ip, 3}).value(), ==, 0b001);
    g_assert_cmpuint((Polynomial{0b101, ip, 3} * Polynomial{0b011, ip, 3}).value(), ==, 0b100);
    g_assert_cmpuint((Polynomial{0b101, ip, 3} * Polynomial{0b100, ip, 3}).value(), ==, 0b010);
    g_assert_cmpuint((Polynomial{0b101, ip, 3} * Polynomial{0b101, ip, 3}).value(), ==, 0b111);
    g_assert_cmpuint((Polynomial{0b101, ip, 3} * Polynomial{0b110, ip, 3}).value(), ==, 0b011);
    g_assert_cmpuint((Polynomial{0b101, ip, 3} * Polynomial{0b111, ip, 3}).value(), ==, 0b110);

    g_assert_cmpuint((Polynomial{0b110, ip, 3} * Polynomial{0b001, ip, 3}).value(), ==, 0b110);
    g_assert_cmpuint((Polynomial{0b110, ip, 3} * Polynomial{0b010, ip, 3}).value(), ==, 0b111);
    g_assert_cmpuint((Polynomial{0b110, ip, 3} * Polynomial{0b011, ip, 3}).value(), ==, 0b001);
    g_assert_cmpuint((Polynomial{0b110, ip, 3} * Polynomial{0b100, ip, 3}).value(), ==, 0b101);
    g_assert_cmpuint((Polynomial{0b110, ip, 3} * Polynomial{0b101, ip, 3}).value(), ==, 0b011);
    g_assert_cmpuint((Polynomial{0b110, ip, 3} * Polynomial{0b110, ip, 3}).value(), ==, 0b010);
    g_assert_cmpuint((Polynomial{0b110, ip, 3} * Polynomial{0b111, ip, 3}).value(), ==, 0b100);

    g_assert_cmpuint((Polynomial{0b111, ip, 3} * Polynomial{0b001, ip, 3}).value(), ==, 0b111);
    g_assert_cmpuint((Polynomial{0b111, ip, 3} * Polynomial{0b010, ip, 3}).value(), ==, 0b101);
    g_assert_cmpuint((Polynomial{0b111, ip, 3} * Polynomial{0b011, ip, 3}).value(), ==, 0b010);
    g_assert_cmpuint((Polynomial{0b111, ip, 3} * Polynomial{0b100, ip, 3}).value(), ==, 0b001);
    g_assert_cmpuint((Polynomial{0b111, ip, 3} * Polynomial{0b101, ip, 3}).value(), ==, 0b110);
    g_assert_cmpuint((Polynomial{0b111, ip, 3} * Polynomial{0b110, ip, 3}).value(), ==, 0b100);
    g_assert_cmpuint((Polynomial{0b111, ip, 3} * Polynomial{0b111, ip, 3}).value(), ==, 0b011);
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "");

    g_test_init(&argc, &argv, nullptr);

    g_test_add_func("/Polynomial/add-subtract1", add_subtract1);
    g_test_add_func("/Polynomial/add-subtract2", add_subtract2);
    g_test_add_func("/Polynomial/add-subtract3", add_subtract3);
    g_test_add_func("/Polynomial/add-subtract4", add_subtract4);
    g_test_add_func("/Polynomial/add-subtract5", add_subtract5);
    g_test_add_func("/Polynomial/multiply1", multiply1);
    g_test_add_func("/Polynomial/multiply2", multiply2);
    g_test_add_func("/Polynomial/multiply3", multiply3);
    g_test_add_func("/Polynomial/multiply4", multiply4);
    g_test_add_func("/Polynomial/multiply5", multiply5);
    g_test_add_func("/Polynomial/multiplication-table", multiplication_table);

    return g_test_run();
}
