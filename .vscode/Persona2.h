#include <gtk/gtk.h>
#include <cairo.h>
#include <math.h>

#define PI 3.14159265358979323846

// Variables globales para almacenar los datos de los rayos
int N = 8;  // Cantidad de rayos (mínimo 4)
int k = 8;  // Cantidad de datos (mínimo 4)
int D[] = {1, 2, 3, 4, 5, 6, 7, 8};  // Datos de prueba
int R = 200; // Radio del círculo

// Callback para dibujar los rayos en la interfaz
static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    int i;
    
    // Obtener el centro del área de dibujo
    int xc = gtk_widget_get_allocated_width(widget) / 2;
    int yc = gtk_widget_get_allocated_height(widget) / 2;

    // Limpiar el área de dibujo (fondo blanco)
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);

    // Dibujar círculo guía (opcional, se puede quitar)
    cairo_set_source_rgb(cr, 0.6, 0.6, 0.6);
    cairo_arc(cr, xc, yc, R, 0, 2 * PI);
    cairo_stroke(cr);

    // Configurar color y grosor de las líneas
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, 2);

    // Dibujar los rayos desde el centro
    for (i = 0; i < k; i++) {
        double angle = (i / (double)N) * 2 * PI;  // Ángulo en radianes
        double length = (D[i] / (double)k) * R;   // Longitud proporcional

        double x_end = xc + length * cos(angle);
        double y_end = yc - length * sin(angle);  // Negativo porque Y crece hacia abajo en GTK

        cairo_move_to(cr, xc, yc);
        cairo_line_to(cr, x_end, y_end);
        cairo_stroke(cr);
    }

    return FALSE;
}

int main(int argc, char *argv[]) {
    GtkBuilder *builder;
    GtkWidget *window;
    GtkWidget *drawing_area;

    gtk_init(&argc, &argv);

    // Cargar la interfaz desde Glade
    builder = gtk_builder_new_from_file("interfaz.glade");

    // Obtener la ventana principal de Glade
    window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Obtener el área de dibujo de Glade y conectar el evento de dibujo
    drawing_area = GTK_WIDGET(gtk_builder_get_object(builder, "drawing_area"));
    g_signal_connect(drawing_area, "draw", G_CALLBACK(on_draw_event), NULL);

    // Mostrar la ventana
    gtk_widget_show_all(window);

    // Ejecutar el bucle principal de GTK
    gtk_main();

    // Limpiar memoria
    g_object_unref(builder);

    return 0;
}
