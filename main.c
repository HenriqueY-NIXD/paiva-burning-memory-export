#include <gtk/gtk.h>
#include <json-glib/json-glib.h>

static void
download (GtkWidget *widget, gpointer   data)
{
  string data = "{\"key1\": \"value1\", \"key2\": \"value2\"}";
  JsonParser *parser = json_parser_new ();
  GError* err = NULL;
  if (!json_parser_load_from_data (parser, data.c_str(), -1, &err)) {
      printf("error in parsing json data %s", err->message);
      g_error_free (err);
      g_object_unref (parser);
      return -1;
  }

  JsonReader *reader = json_reader_new (json_parser_get_root (parser));
  char** members = json_reader_list_members(reader);
  int i = 0;
  while (members[i] != 0) {
      std::string m = members[i];
      if (m == "key1") {
          json_reader_read_member (reader, members[i]);
          std::string value = json_reader_get_string_value (reader);
          json_reader_end_member (reader);
          printf("parse member %s\n", members[i]);
          printf("parse value %s\n", value.c_str());
      }
      i++;
  }

  g_strfreev(members);
  g_object_unref (reader);
  g_object_unref (parser);
}

static void
activate (GtkApplication *app, gpointer user_data)
{
  GtkWidget *window;
  GtkWidget *button;
  GtkWidget *box;
  GtkWidget *image;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Sporta");
  gtk_window_set_default_size (GTK_WINDOW (window), 1280, 720);
  gtk_window_set_resizable (GTK_WINDOW(window), FALSE);
  

  button = gtk_button_new_with_label ("Sporta");
  image = gtk_image_new_from_file ("god.png");

  box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
  gtk_widget_set_size_request(GTK_WIDGET(image), 500, 500);

  g_signal_connect (button, "clicked", G_CALLBACK (download), NULL);

  gtk_window_set_child (GTK_WINDOW (window), box);
  gtk_box_append(GTK_BOX(box), button);
  gtk_box_append(GTK_BOX(box), image);

  gtk_window_present (GTK_WINDOW (window));
}

int
main (int argc, char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.paiva", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
