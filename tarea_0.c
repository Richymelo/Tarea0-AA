#include <gtk/gtk.h>
#include <cairo.h>
#include <math.h>

#define PI 3.14159265358979323846

// Variables globales para almacenar los datos de los rayos
int N = 8;  // Cantidad de rayos (mínimo 4)
int k = 8;  // Cantidad de datos (mínimo 4)
int D[] = {1, 2, 3, 4, 5, 6, 7, 8};  // Datos de prueba

// Callback para dibujar los rayos en la interfaz
gboolean dibujar_area(GtkWidget *area, cairo_t *cr, gpointer user_data) {
    int i;
    
    // Obtener el centro del área de dibujo
    int xc = gtk_widget_get_allocated_width(area) / 2;
    int yc = gtk_widget_get_allocated_height(area) / 2;
    int R = MIN(xc, yc);    // Radio del círculo
    
    // Limpiar el área de dibujo (fondo blanco)
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);

    // Dibujar círculo guía
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

// Para que no se mueva la línea del panel
void fijar_panel(GtkPaned *panel, GParamSpec *pspec, gpointer user_data) {
    const int pos_fijada = 895;    // Posición donde se fija la división
    int current_pos = gtk_paned_get_position(panel);
    if (current_pos != pos_fijada) {
        gtk_paned_set_position(panel, pos_fijada);
    }
}

int main(int argc, char *argv[]) {
    GtkBuilder *builder;        // Utilizado para obtener los objetos de glade
    GtkWidget *ventana;         // La ventana
    GtkWidget *area_circulo;    // El área donde se dibuja el círculo
    GtkWidget *panel;           // El panel que divide el área de dibujo y el área de interacción
    GtkWidget *boton_salida;    // Botón para terminar el programa
    GtkWidget *boton_barajar;   // Botón para barajar los datos

    gtk_init(&argc, &argv);

    // Cargar la interfaz de Glade
    builder = gtk_builder_new_from_file("interfaz.glade");

    // La ventana
    ventana = GTK_WIDGET(gtk_builder_get_object(builder, "ventana"));
    g_signal_connect(ventana, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // El panel divisor
    panel = GTK_WIDGET(gtk_builder_get_object(builder, "division"));
    g_signal_connect(panel, "notify::position", G_CALLBACK(fijar_panel), NULL);

    // El área de dibujo
    area_circulo = GTK_WIDGET(gtk_builder_get_object(builder, "area_circulo"));
    g_signal_connect(area_circulo, "draw", G_CALLBACK(dibujar_area), NULL);

    // El bóton de terminación del programa
    boton_salida = GTK_WIDGET(gtk_builder_get_object(builder, "boton_terminar"));
    g_signal_connect(boton_salida, "clicked", G_CALLBACK(gtk_main_quit), NULL);

    // Mostrar ventana
    gtk_widget_show_all(ventana);
    // Que la ventana utilize toda la pantalla
    gtk_window_fullscreen(GTK_WINDOW(ventana));

    // Correr GTK
    gtk_main();

    // Limpiar la memoria
    g_object_unref(builder);

    return 0;
}