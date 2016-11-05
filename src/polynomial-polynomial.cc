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

#include "polynomial-polynomial.h"

namespace dumbaes {

using namespace internal;

PolynomialPolynomial::PolynomialPolynomial(const Polynomial& a3,
                                           const Polynomial& a2,
                                           const Polynomial& a1,
                                           const Polynomial& a0)
    : m_value(Word{a0.value(), a1.value(), a2.value(), a3.value()})
{
}

PolynomialPolynomial& PolynomialPolynomial::operator+=(
    const PolynomialPolynomial& rhs)
{
    m_value ^= rhs.m_value;
    return *this;
}

PolynomialPolynomial& PolynomialPolynomial::operator*=(
    const PolynomialPolynomial& rhs)
{
    // FIPS 197 equation 4.12
    Word a = m_value;
    Word b = rhs.m_value;
    Word& d = m_value;

    d[0] = ((Polynomial{a[0]} * Polynomial{b[0]}) +
            (Polynomial{a[3]} * Polynomial{b[1]}) +
            (Polynomial{a[2]} * Polynomial{b[2]}) +
            (Polynomial{a[1]} * Polynomial{b[3]})).value();

    d[1] = ((Polynomial{a[1]} * Polynomial{b[0]}) +
            (Polynomial{a[0]} * Polynomial{b[1]}) +
            (Polynomial{a[3]} * Polynomial{b[2]}) +
            (Polynomial{a[2]} * Polynomial{b[3]})).value();

    d[2] = ((Polynomial{a[2]} * Polynomial{b[0]}) +
            (Polynomial{a[1]} * Polynomial{b[1]}) +
            (Polynomial{a[0]} * Polynomial{b[2]}) +
            (Polynomial{a[3]} * Polynomial{b[3]})).value();

    d[3] = ((Polynomial{a[3]} * Polynomial{b[0]}) +
            (Polynomial{a[2]} * Polynomial{b[1]}) +
            (Polynomial{a[1]} * Polynomial{b[2]}) +
            (Polynomial{a[0]} * Polynomial{b[3]})).value();

    return *this;
}

PolynomialPolynomial operator+(PolynomialPolynomial lhs,
                               const PolynomialPolynomial& rhs)
{
    return lhs += rhs;
}

PolynomialPolynomial operator*(PolynomialPolynomial lhs,
                               const PolynomialPolynomial& rhs)
{
    return lhs *= rhs;
}

}
