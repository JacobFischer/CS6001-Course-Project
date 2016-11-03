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

#include <cassert>
#include <cmath>
#include <cstdlib>

namespace dumbaes {

Polynomial::Polynomial(uint8_t value,
                       uint16_t irreducible_polynomial,
                       int characteristic)
    : m_value(value)
    , m_irreducible_polynomial(irreducible_polynomial)
    , m_characteristic(characteristic)
{
#ifndef NDEBUG
    // Verify that the characteristic of the polynomial is compatible
    // with an eight-bit value.
    assert(characteristic > 0);
    assert(characteristic <= 8);

    // Verify that the degree of the polynomial is less than or equal
    // to the characteristic.
    uint8_t characteristic_mask = 0b11111111;
    characteristic_mask >>= (8 - characteristic);
    assert((value & ~characteristic_mask) == 0);
#endif
}

Polynomial& Polynomial::operator+=(const Polynomial& rhs) {
    m_value ^= rhs.m_value;
    return *this;
}

Polynomial& Polynomial::operator-=(const Polynomial& rhs) {
    // Life in a finite field where addition and subtraction are distinct
    // operations: t'would be a life not worth living at all.
    *this += rhs;
    return *this;
}

// Based on https://en.wikipedia.org/wiki/Finite_field_arithmetic#Rijndael.27s_finite_field
// Generalized to work with any characteristic.
Polynomial& Polynomial::operator*=(const Polynomial& rhs) {
    // FIXME: Consider turning this into a template class with the irreducible
    // polynomial and characteristic as template parameters, to prevent attempts
    // to multiply polynomials with different fields at compile time.
    assert(m_irreducible_polynomial == rhs.m_irreducible_polynomial);
    assert(m_irreducible_polynomial != 0);
    assert(m_characteristic == rhs.m_characteristic);
    assert(m_characteristic != 0);

    uint8_t leftmost_bit_mask = 1 << (m_characteristic - 1);
    uint8_t truncate_mask = ~(leftmost_bit_mask << 1);

    uint8_t a = m_value;
    uint8_t b = rhs.m_value;

    m_value = 0;

    while (a != 0 && b != 0) {
        // Polynomial addition
        if (b & 0b00000001)
            m_value ^= a;

        // Divide by x
        b >>= 1;

        // Multiply by x
        bool carry = (a & leftmost_bit_mask);
        a <<= 1;
        a &= truncate_mask;

        if (carry)
            a ^= (m_irreducible_polynomial & truncate_mask);
    }

    return *this;
}

Polynomial& Polynomial::operator/=(const Polynomial& rhs) {
    *this *= multiplicative_inverse(rhs);
    return *this;
}

Polynomial operator+(Polynomial lhs, const Polynomial& rhs) {
    lhs += rhs;
    return lhs;
}

Polynomial operator-(Polynomial lhs, const Polynomial& rhs) {
    lhs -= rhs;
    return lhs;
}

Polynomial operator*(Polynomial lhs, const Polynomial& rhs) {
    lhs *= rhs;
    return lhs;
}

Polynomial operator/(Polynomial lhs, const Polynomial& rhs) {
    lhs /= rhs;
    return lhs;
}

bool operator==(const Polynomial& lhs, const Polynomial& rhs) {
    return lhs.value() == rhs.value();
}

bool operator!=(const Polynomial& lhs, const Polynomial& rhs) {
    return !(lhs == rhs);
}

bool operator<(const Polynomial& lhs, const Polynomial& rhs) {
    return lhs.value() < rhs.value();
}

bool operator>(const Polynomial& lhs, const Polynomial& rhs) {
    return rhs < lhs;
}

bool operator<=(const Polynomial& lhs, const Polynomial& rhs) {
    return !(lhs > rhs);
}

bool operator>=(const Polynomial& lhs, const Polynomial& rhs) {
    return !(lhs < rhs);
}

Polynomial multiplicative_inverse(const Polynomial& p) {
    // We're supposed to extend the extended Euclidean algorithm to cover
    // polynomials and use that here. That would be smart, but I'm not smart.
    // I think there's an easier way... test every polynomial in the field!
    // (Efficiency is obviously not a goal here.)
    for (uint8_t i = 1; i < std::pow(2, p.characteristic()); ++i) {
        Polynomial candidate(i, p.irreducible_polynomial(), p.characteristic());
        Polynomial result = p * candidate;
        if (result.value() == 1)
            return candidate;
    }

    // No multiplicative inverse exists in the field. This is where proper
    // programs would do error handling or something. We'll just crash! Sad!
    std::abort();
}

}
