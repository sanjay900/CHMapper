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
bool canStart = false;
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
    if(scanner == nullptr && canStart) {
        GtkTextIter iter;
        gtk_text_buffer_get_end_iter(buffer, &iter);
        gtk_text_buffer_insert (buffer, &iter, "Started CHMapper, waiting for a Wii Guitar, PS3 Guitar or Raphnet Guitar\n\n", -1);
        scanner = new Scanner(buffer);
        scanner->scan(&inputs);
        //Scan twice, once to pick up wiimotes, and once to pick up their extensions.
        scanner->scan(&inputs);
        tag = g_timeout_add (1, on_timeout, window);
    }
}
void stopScanning (GtkWidget *widget, gpointer *data)
{
    if(scanner != nullptr) {
        GtkTextIter iter;
        gtk_text_buffer_get_end_iter(buffer, &iter);
        gtk_text_buffer_insert (buffer, &iter, "Stopping CHMapper\n", -1);
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
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(view), false);

    buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));
    gtk_text_buffer_insert_at_cursor (buffer, "Click Start to start CHMapper\n", -1);
    
    GtkWidget *start = gtk_button_new_with_label("Start");
    GtkWidget *stop = gtk_button_new_with_label("Stop");

    GtkWidget *box1;
    box1 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start (GTK_BOX (box1), view, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (box1), start, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (box1), stop, FALSE, FALSE, 0);
    gtk_container_add (GTK_CONTAINER(window), box1);
    g_signal_connect (start, "clicked",
		      G_CALLBACK (startScanning), NULL);
    g_signal_connect (stop, "clicked",
		      G_CALLBACK (stopScanning), NULL);
    gtk_widget_show_all(window);
    
    //Find out how to lay this out correctly
    struct libevdev *evdev;
    struct libevdev_uinput *uidev;
    evdev = libevdev_new();
    int err = libevdev_uinput_create_from_device(evdev,
                                                 LIBEVDEV_UINPUT_OPEN_MANAGED,
                                                 &uidev);
    if (err == -ENOENT)
    {
        gtk_text_buffer_insert_at_cursor (buffer, "There was a problem testing the creation of a virtual controller, the below command may help:\n", -1);
        gtk_text_buffer_insert_at_cursor (buffer, "sudo modprobe uinput\n\n", -1);
        return;
    } else if (err == -EACCES)
    {
        gtk_text_buffer_insert_at_cursor (buffer, "There was a problem testing the creation of a virtual controller, the below commands may help: \n", -1);
        gtk_text_buffer_insert_at_cursor (buffer, "sudo modprobe uinput\n\n", -1);
        gtk_text_buffer_insert_at_cursor (buffer, "echo 'KERNEL==\"uinput\", MODE=\"0666\"' | sudo tee /etc/udev/rules.d/50-uinput.rules\n", -1);
        gtk_text_buffer_insert_at_cursor (buffer, "sudo udevadm control --reload-rules\n", -1);
        gtk_text_buffer_insert_at_cursor (buffer, "sudo udevadm trigger\n\n", -1);
        return;
    } 
    canStart = true;
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