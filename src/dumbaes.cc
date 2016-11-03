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

#include <utility>

namespace dumbaes {

// Nk. This would need to be changed to 6 for AES-192 or 8 for AES-256.
static const int key_size = 4;

// Nb. This is always 4 for AES.
static const int block_size = 4;

// Nr. This would need to be changed to 12 for AES-192 or 14 for AES-256.
static const int num_rounds = 10;

// The Rindjael algorithm state is a 4xNb table.
using State = std::array<std::array<uint8_t, block_size>, 4>;

using Word = std::array<uint8_t, 4>;
using RoundKey = std::array<Word, block_size>;
using KeySchedule = std::array<RoundKey, num_rounds+1>;

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

static KeySchedule compute_key_schedule(const Key& key)
{
    // TODO
    return KeySchedule{};
}

static void sub_bytes(State& state)
{
    // TODO
}

static void shift_rows(State& state)
{
    // TODO
}

static void mix_columns(State& state)
{
    // TODO
}

static void add_round_key(State& state, RoundKey&& round_key)
{
    // TODO
}

// See FIPS 197, Fig. 5
Block encrypt_block(const Block& block, const Key& key)
{
    State state = block_to_state(block);
    KeySchedule key_schedule = compute_key_schedule(key);
    add_round_key(state, std::move(key_schedule[0]));

    for (int i = 1; i < num_rounds; i++) {
        sub_bytes(state);
        shift_rows(state);
        mix_columns(state);
        add_round_key(state, std::move(key_schedule[i]));
    }

    sub_bytes(state);
    shift_rows(state);
    add_round_key(state, std::move(key_schedule[num_rounds]));

    return state_to_block(std::move(state));
}

}
