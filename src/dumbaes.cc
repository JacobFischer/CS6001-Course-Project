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

#include <cstring>
#include <utility>

namespace dumbaes {

using namespace internal;

// The Rindjael algorithm state is a 4xNb table.
using State = std::array<std::array<uint8_t, block_size>, 4>;

static State block_to_state(const Block& block)
{
    State state;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            state[i][j] = block[i + 4*j];
    return state;
}

static Block state_to_block(State&& state)
{
    Block block;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            block[i + 4*j] = state[i][j];
    return block;
}

static void substitute_bytes(State& state)
{
    // TODO
}

static void inverse_substitute_bytes(State& state)
{
    // TODO
}

static void shift_rows(State& state)
{
    // TODO
}

static void inverse_shift_rows(State& state)
{
    // TODO
}

static void mix_columns(State& state)
{
    // TODO
}

static void inverse_mix_columns(State& state)
{
    // TODO
}

static void add_round_key(State& state, Word w0, Word w1, Word w2, Word w3)
{
    // TODO
}

// See FIPS 197, Fig. 5
Block encrypt_block(const Block& block, const Key& key)
{
    State state = block_to_state(block);
    KeySchedule w = internal::compute_key_schedule(key);
    add_round_key(state, w[0], w[1], w[2], w[3]);

    for (int i = 1; i < num_rounds; i++) {
        substitute_bytes(state);
        shift_rows(state);
        mix_columns(state);
        add_round_key(state, w[i], w[i+1], w[i+2], w[i+3]);
    }

    substitute_bytes(state);
    shift_rows(state);
    add_round_key(state, w[num_rounds], w[num_rounds+1], w[num_rounds+2], w[num_rounds+3]);

    return state_to_block(std::move(state));
}

// See FIPS 197, Fig. 12
Block decrypt_block(const Block& block, const Key& key)
{
    State state = block_to_state(block);
    KeySchedule w = compute_key_schedule(key);
    add_round_key(state, w[num_rounds], w[num_rounds+1], w[num_rounds+2], w[num_rounds+3]);

    for (int i = num_rounds - 1; i > 0; i--) {
        inverse_shift_rows(state);
        inverse_substitute_bytes(state);
        add_round_key(state, w[i], w[i+1], w[i+2], w[i+3]);
        inverse_mix_columns(state);
    }

    inverse_shift_rows(state);
    inverse_substitute_bytes(state);
    add_round_key(state, w[0], w[1], w[2], w[3]);

    return state_to_block(std::move(state));
}

static Word substitute_word(Word word)
{
    return Word{};
}

static Word rotate_word(Word word)
{
    return Word{};
}

namespace internal {

// FIPS 197 Fig. 11
KeySchedule compute_key_schedule(const Key& key)
{
    KeySchedule schedule;

    // This entire loop should really be a single memcpy. Design failure?
    for (int i = 0; i < key_size; i++) {
        schedule[i][0] = key[4*i];
        schedule[i][1] = key[4*i + 1];
        schedule[i][2] = key[4*i + 2];
        schedule[i][3] = key[4*i + 3];
    }

    for (int i = key_size; i <= block_size * num_rounds; i++) {
    }

    return schedule;
}

} // namespace internal

} // namespace dumbaes
