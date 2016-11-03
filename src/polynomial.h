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

#pragma once

#include <cstdint>

namespace dumbaes {

// Represents a polynomial in a finite field GF(2^n), with n<=8.
//
// This class basically only exists to provide classy operator overloading.
// It's probably useless overkill, but I like it.
// Reference: http://stackoverflow.com/questions/4421706/operator-overloading/4421719
class Polynomial
{
  public:
    explicit Polynomial(uint8_t value,
                        uint16_t irreducible_polynomial = aes_irreducible_polynomial,
                        int characteristic = 8);

    // The bit representation of this polynomial.
    uint8_t value() const { return m_value; }
    // The irreducible polynomial that defines the field this polynomial exists in.
    uint16_t irreducible_polynomial() const { return m_irreducible_polynomial; }
    // The n in GF(2^n).
    int characteristic() const { return m_characteristic; }

    Polynomial& operator+=(const Polynomial&);
    Polynomial& operator-=(const Polynomial&);
    Polynomial& operator*=(const Polynomial&);
    Polynomial& operator/=(const Polynomial&);

    static const uint16_t aes_irreducible_polynomial = 0b100011011;

  private:
    uint8_t m_value;
    uint16_t m_irreducible_polynomial;
    int m_characteristic;
};

// The one! The only! Our reason for being! MULTINV!
Polynomial multiplicative_inverse(const Polynomial&);

Polynomial operator+(Polynomial, const Polynomial&);
Polynomial operator-(Polynomial, const Polynomial&);
Polynomial operator*(Polynomial, const Polynomial&);
Polynomial operator/(Polynomial, const Polynomial&);

bool operator==(const Polynomial&, const Polynomial&);
bool operator!=(const Polynomial&, const Polynomial&);
bool operator<(const Polynomial&, const Polynomial&);
bool operator>(const Polynomial&, const Polynomial&);
bool operator<=(const Polynomial&, const Polynomial&);
bool operator>=(const Polynomial&, const Polynomial&);

}
