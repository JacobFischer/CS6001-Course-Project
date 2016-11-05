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
#include <gio/gio.h>
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

static char *
read_ciphertext (void)
{
  GError *error = NULL;
  char *ciphertext;
  gsize ciphertext_length;

  if (!g_file_get_contents (ciphertext_filename,
                            &ciphertext,
                            &ciphertext_length,
                            &error))
    {
      g_fprintf (stderr, "Failed to read ciphertext file: %s\n", error->message);
      g_error_free (error);
      return NULL;
    }

  // TODO: Remove length restriction when switching to CBC.
  if (ciphertext_length != 17)
    {
      g_fprintf (stderr,
                 "Read %" G_GSIZE_FORMAT " bytes from %s, but ciphertext file must be exactly 17 bytes long (16 plus EOF)\n",
                 ciphertext_length, ciphertext_filename);
      g_free (ciphertext);
      return NULL;
    }

  return ciphertext;
}

static char *
read_plaintext (void)
{
  GError *error = NULL;
  char *plaintext;
  gsize plaintext_length;

  if (!g_file_get_contents (plaintext_filename,
                            &plaintext,
                            &plaintext_length,
                            &error))
    {
      g_fprintf (stderr, "Failed to read plaintext file: %s\n", error->message);
      g_error_free (error);
      return NULL;
    }

  // TODO: Remove length restriction when switching to CBC.
  if (plaintext_length != 17)
    {
      g_fprintf (stderr,
                 "Read %" G_GSIZE_FORMAT " bytes from %s, but plaintext file must be exactly 17 bytes long (16 plus EOF)\n",
                 plaintext_length, plaintext_filename);
      g_free (plaintext);
      return NULL;
    }

  return plaintext;
}

static char *
read_private_key (void)
{
  GError *error = NULL;
  char *key;
  gsize key_length;

  if (!g_file_get_contents (key_filename,
                            &key,
                            &key_length,
                            &error))
    {
      g_fprintf (stderr, "Failed to read ciphertext file: %s\n", error->message);
      g_error_free (error);
      return NULL;
    }

  if (key_length != 17)
    {
      g_fprintf (stderr,
                 "Read %" G_GSIZE_FORMAT " bytes from %s, but key file must be exactly 17 bytes long (16 plus EOF)\n",
                 key_length, key_filename);
      g_free (key);
      return NULL;
    }

  return key;
}

static int
write_to_output_file (const unsigned char *output)
{
  GFile *file;
  GFileIOStream *iostream;
  GOutputStream *output_stream;
  gsize bytes_written;
  int ret = EXIT_FAILURE;
  GError *error = NULL;

  file = g_file_new_for_path (output_filename);
  iostream = g_file_create_readwrite (file,
                                      G_FILE_CREATE_REPLACE_DESTINATION,
                                      NULL,
                                      &error);
  if (iostream == NULL)
    {
      if (g_error_matches (error, G_IO_ERROR, G_IO_ERROR_EXISTS))
        {
          g_clear_error (&error);
          iostream = g_file_open_readwrite (file, NULL, &error);
          if (iostream == NULL)
            {
              g_fprintf (stderr, "Failed to open %s for writing: %s\n", output_filename, error->message);
              g_error_free (error);
              goto out;
            }
        }
      else
        {
          g_fprintf (stderr, "Failed to create %s for writing: %s\n", output_filename, error->message);
          g_error_free (error);
          goto out;
        }
    }

  output_stream = g_io_stream_get_output_stream (G_IO_STREAM (iostream));

  // TODO: When switching to CBC, this function should receive a NULL-terminated
  // const char * instead of a const unsigned char *, and the third argument to
  // g_output_stream_write_all() should use strlen() rather than be hardcoded.
  if (!g_output_stream_write_all (output_stream,
                                  output,
                                  16,
                                  &bytes_written,
                                  NULL,
                                  &error))
    {
      g_fprintf (stderr, "Failed to write to %s: %s\n", output_filename, error->message);
      g_error_free (error);
      goto out;
    }

  ret = EXIT_SUCCESS;

out:
  g_object_unref (file);

  if (iostream != NULL)
    g_object_unref (iostream);

  return ret;
}

static int
decrypt_file (void)
{
  char *ciphertext = NULL;
  char *key = NULL;
  unsigned char *plaintext = NULL;
  int ret = EXIT_FAILURE;

  ciphertext = read_ciphertext ();
  if (ciphertext == NULL)
    goto out;

  key = read_private_key ();
  if (key == NULL)
    goto out;

  // TODO: Use CBC here instead.
  plaintext = dumbaes_128_decrypt_block ((unsigned char *)ciphertext,
                                         (unsigned char *)key);
  ret = write_to_output_file (plaintext);

out:
  free (plaintext);

  g_free (ciphertext);
  g_free (key);

  return ret;
}

static int
encrypt_file (void)
{
  char *plaintext = NULL;
  char *key = NULL;
  unsigned char *ciphertext = NULL;
  int ret = EXIT_FAILURE;

  plaintext = read_plaintext ();
  if (plaintext == NULL)
    goto out;

  key = read_private_key ();
  if (key == NULL)
    goto out;

  // TODO: Use CBC here instead.
  ciphertext = dumbaes_128_encrypt_block ((unsigned char *)plaintext,
                                          (unsigned char *)key);
  ret = write_to_output_file (ciphertext);

out:
  free (ciphertext);

  g_free (plaintext);
  g_free (key);

  return ret;
}

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

  if (ciphertext_filename != NULL)
    ret = decrypt_file ();
  else
    ret = encrypt_file ();

out:
  g_option_context_free (context);

  g_free (ciphertext_filename);
  g_free (plaintext_filename);
  g_free (key_filename);
  g_free (output_filename);

  return ret;
}
