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
GtkWidget *window;
Scanner *scanner = nullptr;
guint tag;
std::list<Input *> inputs;
static gboolean
on_timeout (gpointer user_data) {
    scanner->findNew(&inputs);
    for (auto x: inputs) {
        x->tick();
    }
    return true;
}
void startScanning (GtkWidget *widget, gpointer *data)
{
    std::cout << "start" << std::endl;
    scanner = new Scanner(buffer);
    scanner->scan(&inputs);
    //Scan twice, once to pick up wiimotes, and once to pick up their extensions.
    scanner->scan(&inputs);
    tag = g_timeout_add (100, on_timeout, window);
}
void stopScanning (GtkWidget *widget, gpointer *data)
{
    if(scanner != nullptr) {
        for (auto in: inputs) {
            in->disconnect();
        }
        inputs.clear();

        g_source_remove(tag);
        scanner = nullptr;
    }
}
static void
activate(GtkApplication *app,
    gpointer user_data) {

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "CHMapper");
    GtkWidget *view;

    view = gtk_text_view_new ();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(view), false);

    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));
    GtkWidget *start = gtk_button_new_with_label("Start");
    GtkWidget *stop = gtk_button_new_with_label("Stop");
    //Find out how to lay this out correctly
    gtk_container_add (GTK_CONTAINER(window), view);
    gtk_container_add (GTK_CONTAINER(window), start);
    gtk_container_add (GTK_CONTAINER(window), stop);
    g_signal_connect (start, "clicked",
		      G_CALLBACK (startScanning), NULL);
    g_signal_connect (stop, "clicked",
		      G_CALLBACK (stopScanning), NULL);
    gtk_widget_show_all(window);
    gtk_text_buffer_insert_at_cursor (buffer, "Started CHMapper, waiting for a Wii Guitar, PS3 Guitar or Raphnet Guitar\n\n", -1);
    
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