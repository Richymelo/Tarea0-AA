#include <gtk/gtk.h>
#include <cairo.h>
#include <math.h>

// Callback for drawing on a drawing area
static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    // Set color and draw a circle
    cairo_set_source_rgb(cr, 0.6, 0.6, 0.6);
    cairo_arc(cr, 100, 100, 50, 0, 2 * M_PI);
    cairo_fill(cr);
    return FALSE;
}

int main(int argc, char *argv[]) {
    GtkBuilder *builder;
    GtkWidget *window;
    GtkWidget *drawing_area;

    gtk_init(&argc, &argv);

    // Load UI from Glade file
    builder = gtk_builder_new_from_file("interfaz.glade");

    // Get the main window from the Glade file
    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Get the drawing area from the Glade file
    drawing_area = GTK_WIDGET(gtk_builder_get_object(builder, "drawing_area"));
    g_signal_connect(drawing_area, "draw", G_CALLBACK(on_draw_event), NULL);

    // Show the window
    gtk_widget_show_all(window);

    // Run the GTK main loop
    gtk_main();

    // Clean up
    g_object_unref(builder);

    return 0;
}