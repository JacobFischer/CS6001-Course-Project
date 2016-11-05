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
#include <glib/gstdio.h>
#include <glib.h>
#include <locale.h>
#include <stdlib.h>

static char *ciphertext_filename = NULL;
static char *plaintext_filename = NULL;
static char *key_filename = NULL;
static char *output_filename = NULL;

static const GOptionEntry option_entries[] =
{
  { "decrypt", 'd', 0, G_OPTION_ARG_FILENAME, &ciphertext_filename,
    "File to decrypt", "FILE" },
  { "encrypt", 'e', 0, G_OPTION_ARG_FILENAME, &plaintext_filename,
    "File to encrypt", "FILE" },
  { "key", 'k', 0, G_OPTION_ARG_FILENAME, &key_filename,
    "File containing private key", "FILE" },
  { "output", 'o', 0, G_OPTION_ARG_FILENAME, &output_filename,
    "File to use for output", "FILE" },
  { NULL }
};

int
main (int argc, char **argv)
{
  GError *error = NULL;
  GOptionContext *context;
  int ret = EXIT_FAILURE;

  setlocale (LC_ALL, "");

  context = g_option_context_new ("- dumbaes example program");
  g_option_context_add_main_entries (context, option_entries, NULL);

  if (!g_option_context_parse (context, &argc, &argv, &error))
    {
      g_fprintf (stderr, "option parsing failed: %s\n", error->message);
      g_error_free (error);
      goto out;
    }

  if (output_filename == NULL)
    {
      g_fprintf (stderr, "You must specify an output file with -o\n");
      goto out;
    }

  if (key_filename == NULL)
    {
      g_fprintf (stderr, "You must specify a file to use for the private key with -k\n");
      goto out;
    }

  if (ciphertext_filename == NULL && plaintext_filename == NULL)
    {
      g_fprintf (stderr, "You must specify a file to encrypt or decrypt with -d or -e\n");
      goto out;
    }

  if (ciphertext_filename != NULL && plaintext_filename != NULL)
    {
      g_fprintf (stderr, "You must specify only one of -d/--decrypt or -e/--encrypt\n");
      goto out;
    }

  g_printf ("Ciphertext filename=%s\n", ciphertext_filename);
  g_printf ("Plaintext filename=%s\n", plaintext_filename);
  g_printf ("Key filename=%s\n", key_filename);
  g_printf ("Output filename=%s\n", output_filename);

  ret = EXIT_SUCCESS;

out:
  g_option_context_free (context);

  g_free (ciphertext_filename);
  g_free (plaintext_filename);
  g_free (key_filename);
  g_free (output_filename);

  return ret;
}
