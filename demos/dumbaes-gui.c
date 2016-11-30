/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
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
#include <glib.h>
#include <gtk/gtk.h>
#include <stdlib.h>

static GtkWidget *window = NULL;
static GtkWidget *result_label = NULL;
static GFile *input_file = NULL;
static GFile *key_file = NULL;

// TODO: Figure out how to format an arbitary-length of data when implementing CBC.
#define BLOCK_FORMAT "%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x"

static void
show_error (const char *error)
{
  char *error_markup = g_strdup_printf ("<span color=\"#FF0000\">%s</span>", error);
  gtk_label_set_markup (GTK_LABEL (result_label), error_markup);
  gtk_widget_show (result_label);
  g_free (error_markup);
}

static char *
read_file (GFile *file)
{
  GError *error = NULL;
  char *contents;
  gsize contents_length;

  if (!g_file_get_contents (g_file_get_path (file),
                            &contents,
                            &contents_length,
                            &error))
    {
      char *message = g_strdup_printf ("Failed to read %s: %s\n",
                                       g_file_get_path (file),
                                       error->message);
      show_error (message);
      g_free (message);
      g_error_free (error);
      return NULL;
    }

  // TODO: Remove length restriction when switching to CBC.
  if (contents_length < 16)
    {
      char *message = g_strdup_printf ("Read %" G_GSIZE_FORMAT " bytes from %s, but file must be 16 bytes long\n",
                                        contents_length, g_file_get_path (file));
      show_error (message);
      g_free (contents);
      g_free (message);
      return NULL;
    }

  return contents;
}

static void
input_file_set_cb (GtkFileChooserButton *widget)
{
  if (input_file != NULL)
    g_object_unref (input_file);

  input_file = gtk_file_chooser_get_file (GTK_FILE_CHOOSER (widget));
}

static void
key_file_set_cb (GtkFileChooserButton *widget)
{
  if (key_file != NULL)
    g_object_unref (key_file);

  key_file = gtk_file_chooser_get_file (GTK_FILE_CHOOSER (widget));
}

static void
encrypt_button_activate_cb (GtkButton *button)
{
  char *input = NULL;
  char *key = NULL;
  unsigned char *ciphertext = NULL;
  char *ciphertext_string = NULL;

  if (input_file == NULL || key_file == NULL)
    {
      show_error ("Dumbaes, you need to select both input file and key file!");
      return;
    }

  input = read_file (input_file);
  if (input == NULL)
    goto out;

  key = read_file (key_file);
  if (key == NULL)
    goto out;

  // TODO: Use CBC here instead.
  ciphertext = dumbaes_128_encrypt_block ((unsigned char *)input,
                                          (unsigned char *)key);
  ciphertext_string = g_strdup_printf ("Encrypted text: " BLOCK_FORMAT,
                                       ciphertext[0], ciphertext[1], ciphertext[2], ciphertext[3],
                                       ciphertext[4], ciphertext[5], ciphertext[6], ciphertext[7],
                                       ciphertext[8], ciphertext[9], ciphertext[10], ciphertext[11],
                                       ciphertext[12], ciphertext[13], ciphertext[14], ciphertext[15]);

  gtk_label_set_text (GTK_LABEL (result_label), ciphertext_string);
  gtk_widget_show (result_label);

out:
  free (ciphertext);
  g_free (ciphertext_string);
  g_free (input);
  g_free (key);
}

static void
decrypt_button_activate_cb (GtkButton *button)
{
  char *input = NULL;
  char *key = NULL;
  unsigned char *plaintext = NULL;
  char *plaintext_string = NULL;

  if (input_file == NULL || key_file == NULL)
    {
      show_error ("Dumbaes, you need to select both input file and key file!");
      return;
    }

  input = read_file (input_file);
  if (input == NULL)
    goto out;

  key = read_file (key_file);
  if (key == NULL)
    goto out;

  // TODO: Use CBC here instead.
  plaintext = dumbaes_128_decrypt_block ((unsigned char *)input,
                                         (unsigned char *)key);
  plaintext_string = g_strdup_printf ("Encrypted text: " BLOCK_FORMAT,
                                      plaintext[0], plaintext[1], plaintext[2], plaintext[3],
                                      plaintext[4], plaintext[5], plaintext[6], plaintext[7],
                                      plaintext[8], plaintext[9], plaintext[10], plaintext[11],
                                      plaintext[12], plaintext[13], plaintext[14], plaintext[15]);

  gtk_label_set_text (GTK_LABEL (result_label), plaintext_string);
  gtk_widget_show (result_label);

out:
  free (plaintext);
  g_free (plaintext_string);
  g_free (input);
  g_free (key);
}

static void
save_button_clicked_cb (void)
{
}

static void
create_ui (void)
{
  GtkWidget *vbox;
  GtkWidget *hbox;
  GtkWidget *widget;
  GtkWidget *headerbar;
  GtkStyleContext *context;

  headerbar = gtk_header_bar_new ();
  gtk_header_bar_set_title (GTK_HEADER_BAR (headerbar), g_get_application_name ());
  gtk_header_bar_set_show_close_button (GTK_HEADER_BAR (headerbar), TRUE);
  gtk_window_set_titlebar (GTK_WINDOW (window), headerbar);
  gtk_widget_show (headerbar);

  widget = gtk_button_new_with_mnemonic ("_Save");
  g_signal_connect (widget, "clicked",
                    G_CALLBACK (save_button_clicked_cb), NULL);
  gtk_header_bar_pack_start (GTK_HEADER_BAR (headerbar), widget);
  gtk_widget_show (widget);

  context = gtk_widget_get_style_context (widget);
  gtk_style_context_add_class (context, "text-button");
  gtk_style_context_add_class (context, "suggested-action");

  vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 12);
  g_object_set (vbox, "margin", 12, NULL);
  gtk_container_add (GTK_CONTAINER (window), vbox);

  hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 6);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);

  widget = gtk_label_new_with_mnemonic ("_Input file:");
  gtk_box_pack_start (GTK_BOX (hbox), widget, FALSE, FALSE, 0);

  widget = gtk_file_chooser_button_new ("Select input file",
                                        GTK_FILE_CHOOSER_ACTION_OPEN);
  g_signal_connect (widget, "file-set",
                    G_CALLBACK (input_file_set_cb), NULL);
  gtk_box_pack_end (GTK_BOX (hbox), widget, FALSE, FALSE, 0);

  hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 6);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);

  widget = gtk_label_new_with_mnemonic ("Encryption _key file:");
  gtk_box_pack_start (GTK_BOX (hbox), widget, FALSE, FALSE, 0);

  widget = gtk_file_chooser_button_new ("Select encryption key file",
                                        GTK_FILE_CHOOSER_ACTION_OPEN);
  g_signal_connect (widget, "file-set",
                    G_CALLBACK (key_file_set_cb), NULL);
  gtk_box_pack_end (GTK_BOX (hbox), widget, FALSE, FALSE, 0);

  hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 6);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);

  widget = gtk_button_new_with_mnemonic ("_Encrypt");
  g_signal_connect (widget, "clicked",
                    G_CALLBACK (encrypt_button_activate_cb), NULL);
  gtk_box_pack_start (GTK_BOX (hbox), widget, FALSE, FALSE, 24);

  widget = gtk_button_new_with_mnemonic ("_Decrypt");
  g_signal_connect (widget, "clicked",
                    G_CALLBACK (decrypt_button_activate_cb), NULL);
  gtk_box_pack_end (GTK_BOX (hbox), widget, FALSE, FALSE, 24);

  result_label = gtk_label_new (NULL);
  gtk_widget_set_no_show_all (result_label, TRUE);
  gtk_box_pack_end (GTK_BOX (vbox), result_label, FALSE, FALSE, 6);

  gtk_widget_show_all (vbox);
}

static void
application_activate_cb (GApplication *application)
{
  if (window != NULL)
    return;

  window = gtk_application_window_new (GTK_APPLICATION (application));
  create_ui ();
  gtk_window_present (GTK_WINDOW (window));
}

int
main (int argc, char **argv)
{
  GtkApplication *application;
  int ret;

  g_set_application_name ("Dumb AES Demo");

  application = gtk_application_new ("edu.mst.dumbaes", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (application, "activate",
                    G_CALLBACK (application_activate_cb), NULL);

  ret = g_application_run (G_APPLICATION (application), argc, argv);

  g_object_unref (application);
  if (input_file != NULL)
    g_object_unref (input_file);
  if (key_file != NULL)
    g_object_unref (key_file);

  return ret;
}
