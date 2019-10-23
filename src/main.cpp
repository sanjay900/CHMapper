#include <iostream>
#include <libudev.h>
#include <chrono>
#include <thread>
#include <fcntl.h>
#include <zconf.h>
#include "scanner.h"
#include "input/Input.h"
#include <list>
#include <iostream>
#include <gtk/gtk.h>

GtkTextBuffer *buffer;
Scanner *scanner;
std::list<Input *> inputs;
static gboolean
on_timeout (gpointer user_data) {
    scanner->findNew(&inputs);
    for (auto x: inputs) {
        x->tick();
    }
}
static void
activate(GtkApplication *app,
    gpointer user_data) {
    GtkWidget *window;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "CHMapper");
    GtkWidget *view;

    view = gtk_text_view_new ();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(view), false);

    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));
    gtk_container_add (GTK_CONTAINER(window), view);
    gtk_widget_show_all(window);
    gtk_text_buffer_insert_at_cursor (buffer, "Started CHMapper, waiting for a Wii Guitar, PS3 Guitar or Raphnet Guitar\n\n", -1);
    
    scanner = new Scanner(buffer);
    scanner->scan(&inputs);
    //Scan twice, once to pick up wiimotes, and once to pick up their extensions.
    scanner->scan(&inputs);
    g_timeout_add (100, on_timeout, window);
}

int main(int argc, char *argv[])
{
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example",
        G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate",
        G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return 0;
}