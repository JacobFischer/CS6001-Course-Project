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
#include <string.h>

static GtkWidget *window = NULL;
static GtkWidget *result_label = NULL;
static GtkWidget *save_button = NULL;
static GFile *input_file = NULL;
static GFile *key_file = NULL;
static unsigned char *result = NULL;
static gboolean displaying_ciphertext = FALSE;
static gsize length;

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
read_file (GFile *file, gboolean is_key)
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
    
  if (!is_key)
    {
      length = contents_length;
    }      

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
encrypt_button_activate_cb (void)
{
  char *input = NULL;
  char *key = NULL;
  char *display_string = NULL;
  unsigned char *ciphertext_no_iv = NULL;
  unsigned char *iv_encrypted = NULL;
  unsigned char *iv_plain = NULL;

  if (input_file == NULL || key_file == NULL)
    {
      show_error ("Dumbaes, you need to select both input file and key file!");
      return;
    }

  input = read_file (input_file, FALSE);
  if (input == NULL)
    goto out;

  key = read_file (key_file, TRUE);
  if (key == NULL)
    goto out;

  g_clear_pointer (&result, free);
  
  iv_plain = dumbaes_128_generate_iv ();
  ciphertext_no_iv = dumbaes_128_encrypt_cbc ((unsigned char *)input,
                                              &length, 
                                              (unsigned char *)key,
                                              (unsigned char *)iv_plain);
  iv_encrypted = dumbaes_128_encrypt_block ((unsigned char *)iv_plain,
                                            (unsigned char *)key);
  //Append IV to ciphertext so ciphertext and IV are conveniently stored in one 
  //  location. No need to remember which IV goes with which ciphertext
  result = (unsigned char*)malloc(length + 16);
  memcpy(result, ciphertext_no_iv, length);
  memcpy(result+length, iv_encrypted, 16);
  length += 16;

  display_string = g_strdup_printf ("Encryption: DONE :)");
  gtk_label_set_text (GTK_LABEL (result_label), display_string);
  gtk_widget_show (result_label);

  gtk_widget_set_sensitive (save_button, TRUE);
  displaying_ciphertext = TRUE;

out:
  free (ciphertext_no_iv);
  free (iv_encrypted);
  free (iv_plain);
  
  g_free (display_string);
  g_free (input);
  g_free (key);
}

static void
decrypt_button_activate_cb (void)
{
  char *input = NULL;
  char *key = NULL;
  char *display_string = NULL;
  unsigned char *iv_encrypted = NULL;
  unsigned char *iv_plain = NULL;

  if (input_file == NULL || key_file == NULL)
    {
      show_error ("Dumbaes, you need to select both input file and key file!");
      return;
    }

  input = read_file (input_file, FALSE);
  if (input == NULL)
    goto out;

  key = read_file (key_file, TRUE);
  if (key == NULL)
    goto out;

  g_clear_pointer (&result, free);
  
  length -= 16;
  iv_encrypted = (unsigned char*)malloc(16);
  memcpy(iv_encrypted, input+length, 16);
  iv_plain = dumbaes_128_decrypt_block((unsigned char *)iv_encrypted,
                                       (unsigned char *)key);
  result = dumbaes_128_decrypt_cbc ((unsigned char *)input,
                                    &length, 
                                    (unsigned char *)key,
                                    (unsigned char *)iv_plain);

  display_string = g_strdup_printf ("Decryption: DONE :)");
  gtk_label_set_text (GTK_LABEL (result_label), display_string);
  gtk_widget_show (result_label);

  gtk_widget_set_sensitive (save_button, TRUE);
  displaying_ciphertext = FALSE;

out:
  free (iv_encrypted);
  free (iv_plain);

  g_free (display_string);
  g_free (input);
  g_free (key);
}

static void
write_to_output_file (const char *output_filename)
{
  GFile *file;
  GFileIOStream *iostream;
  gsize bytes_written;
  char *error_message;
  GError *error = NULL;

  file = g_file_new_for_path (output_filename);
  iostream = g_file_replace_readwrite (file, NULL, FALSE, 0, NULL, &error);
  if (iostream == NULL)
    {
      error_message = g_strdup_printf ("Failed to open %s for writing: %s\n", output_filename, error->message);
      show_error (error_message);
      g_free (error_message);
      g_error_free (error);
      goto out;
    }

  // Beware that |result| can contain embedded NULLs.
  if (!g_output_stream_write_all (g_io_stream_get_output_stream (G_IO_STREAM (iostream)),
                                  result,
                                  length,
                                  &bytes_written,
                                  NULL,
                                  &error))
    {
      error_message = g_strdup_printf ("Failed to write to %s: %s\n", output_filename, error->message);
      show_error (error_message);
      g_free (error_message);
      g_error_free (error);
    }

out:
  g_object_unref (file);

  if (iostream != NULL)
    g_object_unref (iostream);
}

static void
save_button_clicked_cb (void)
{
  GtkWidget *dialog;
  char *filename;
  gint response;

  dialog = gtk_file_chooser_dialog_new (displaying_ciphertext ? "Save Ciphertext" : "Save Plaintext",
                                        GTK_WINDOW (window),
                                        GTK_FILE_CHOOSER_ACTION_SAVE,
                                        "_Cancel",
                                        GTK_RESPONSE_CANCEL,
                                        "_Save",
                                        GTK_RESPONSE_ACCEPT,
                                        NULL);
  gtk_file_chooser_set_do_overwrite_confirmation (GTK_FILE_CHOOSER (dialog), TRUE);
  gtk_file_chooser_set_current_name (GTK_FILE_CHOOSER (dialog),
                                     displaying_ciphertext ? "My ciphertext" : "My plaintext");

  response = gtk_dialog_run (GTK_DIALOG (dialog));
  if (response == GTK_RESPONSE_ACCEPT)
    {
      filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
      write_to_output_file (filename);
      g_free (filename);
    }

  gtk_widget_destroy (dialog);
}

static void
create_ui (void)
{
  GtkWidget *vbox;
  GtkWidget *hbox;
  GtkWidget *widget;
  GtkStyleContext *context;

  widget = gtk_header_bar_new ();
  gtk_header_bar_set_title (GTK_HEADER_BAR (widget), g_get_application_name ());
  gtk_header_bar_set_show_close_button (GTK_HEADER_BAR (widget), TRUE);
  gtk_window_set_titlebar (GTK_WINDOW (window), widget);
  gtk_widget_show (widget);

  save_button = gtk_button_new_with_mnemonic ("_Save");
  g_signal_connect (save_button, "clicked",
                    G_CALLBACK (save_button_clicked_cb), NULL);
  gtk_header_bar_pack_start (GTK_HEADER_BAR (widget), save_button);
  gtk_widget_set_sensitive (save_button, FALSE);
  gtk_widget_show (save_button);

  context = gtk_widget_get_style_context (save_button);
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

  result_label = gtk_label_new (NULL);
  gtk_widget_set_no_show_all (result_label, TRUE);
  gtk_box_pack_start (GTK_BOX (vbox), result_label, FALSE, FALSE, 6);

  hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 6);
  gtk_box_pack_end (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);

  widget = gtk_button_new_with_mnemonic ("_Encrypt");
  g_signal_connect (widget, "clicked",
                    G_CALLBACK (encrypt_button_activate_cb), NULL);
  gtk_box_pack_start (GTK_BOX (hbox), widget, FALSE, FALSE, 24);

  widget = gtk_button_new_with_mnemonic ("_Decrypt");
  g_signal_connect (widget, "clicked",
                    G_CALLBACK (decrypt_button_activate_cb), NULL);
  gtk_box_pack_end (GTK_BOX (hbox), widget, FALSE, FALSE, 24);

  gtk_widget_show_all (vbox);
}

static void
application_startup_cb (GApplication *application)
{
  const char * const quit_accels[] = { "<Primary>Q", "<Primary>W", NULL };
  gtk_application_set_accels_for_action (GTK_APPLICATION (application), "app.quit", quit_accels);
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

static void
quit_cb (GSimpleAction *action,
         GVariant      *parameter,
         gpointer       user_data)
{
  GApplication *application = G_APPLICATION (user_data);
  g_application_quit (application);
}

int
main (int argc, char **argv)
{
  GtkApplication *application;
  int ret;
  const GActionEntry entries[] = {
    {"quit", quit_cb, NULL, NULL, NULL}
  };

  g_set_application_name ("Dumb AES Demo");

  application = gtk_application_new ("edu.mst.dumbaes", G_APPLICATION_FLAGS_NONE);
  g_action_map_add_action_entries (G_ACTION_MAP (application), entries, G_N_ELEMENTS (entries), application);
  g_signal_connect (application, "startup",
                    G_CALLBACK (application_startup_cb), application);
  g_signal_connect (application, "activate",
                    G_CALLBACK (application_activate_cb), application);
  ret = g_application_run (G_APPLICATION (application), argc, argv);

  g_object_unref (application);
  if (input_file != NULL)
    g_object_unref (input_file);
  if (key_file != NULL)
    g_object_unref (key_file);
  if (result != NULL)
    free (result);

  return ret;
}
