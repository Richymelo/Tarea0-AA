#include <gtk/gtk.h>
#include <cairo.h>
#include <math.h>     // Para calcular los rayos
#include <stdlib.h>  // Para utilizar rand()
#include <time.h>    // Para utilizar rand()
#include "Datos_usuario.h"

#define PI 3.14159265358979323846

// Para que no se mueva la línea del panel
void fijar_panel(GtkPaned *panel, GParamSpec *pspec, gpointer user_data) {
    const int pos_fijada = 895;    // Posición donde se fija la división
    int current_pos = gtk_paned_get_position(panel);
    if (current_pos != pos_fijada) {
        gtk_paned_set_position(panel, pos_fijada);
    }
}
// Barajar datos de un vector (con el algoritmo de Fisher-Yates o Knuth)
void barajar_datos(int *datos, int size) {
    // Si el vector es de 1 solo valor o menos
    if (datos == NULL || size <= 1) return; 

    for (int i = size - 1; i > 0; i--) {
        // Se escoge una posición aleatoria
        int j = rand() % (i + 1);
        // Se cambian de posición el valor en la posición aleatoria y el de la posición actual
        int temp = datos[i];
        datos[i] = datos[j];
        datos[j] = temp;
    }
}
// Obtener datos
void desplegar_datos(GtkButton *button, gpointer user_data) {
    GtkBuilder *builder = GTK_BUILDER(user_data);

    // Número de rayos ingresados por el usuario
    GtkWidget *cantidad_rayos = GTK_WIDGET(gtk_builder_get_object(builder, "cantidad_rayos"));
    int N = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(cantidad_rayos));
    // Cantidad de datos ingresados por el usuario
    GtkWidget *cantidad_datos = GTK_WIDGET(gtk_builder_get_object(builder, "cantidad_datos"));
    int k = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(cantidad_datos));

    GdkRGBA color_1;
    GtkWidget *primer_color = GTK_WIDGET(gtk_builder_get_object(builder, "color_1"));
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(primer_color), &color_1);

    GdkRGBA color_2;
    GtkWidget *segundo_color = GTK_WIDGET(gtk_builder_get_object(builder, "color_2"));
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(segundo_color), &color_2);

    // Datos globales del usuario
    datos_global.N = N;
    datos_global.k = k;
    datos_global.color_1 = color_1;
    datos_global.color_2 = color_2;
    
    // Limpiar memoria si ya había un vector
    if (datos_global.D != NULL) {
        free(datos_global.D);
    }
    
    // Crear un vector con k espacios en memoria dinámica
    datos_global.D = malloc(sizeof(int) * k);
    // Si el número ingresado es muy grande
    if (datos_global.D == NULL) {
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
        "No se puede asignar memoria para esta cantidad de dígitos. Por favor ingrese un valor más pequeño.");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return;  // El usuario puede volver a intentar
}
    for (int i = 0; i < k; i++) {
        datos_global.D[i] = i + 1;  // Llenar el vector con datos desde 1 hasta k
    }
    barajar_datos(datos_global.D, k);   // Cambiar de orden los valores en el vector
    
    // Volver a dibujar el círculo, esta vez con los rayos
    GtkWidget *area_circulo = GTK_WIDGET(gtk_builder_get_object(builder, "area_circulo"));
    gtk_widget_queue_draw(area_circulo);
}
// Mostrar círculo y rayos
gboolean dibujar_area(GtkWidget *area, cairo_t *cr, gpointer user_data) {
    // Obtener el centro del área de dibujo
    int xc = gtk_widget_get_allocated_width(area) / 2;
    int yc = gtk_widget_get_allocated_height(area) / 2;
    int R = MIN(xc, yc); // Radio del círculo
    
    // Limpiar el área de dibujo (fondo blanco)
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);
    
    // Dibujar círculo guía
    cairo_set_source_rgb(cr, 0.6, 0.6, 0.6);
    cairo_arc(cr, xc, yc, R, 0, 2 * PI);
    cairo_stroke(cr);
    
    // Si el usuario ha agregado datos, dibujar los rayos
    if (datos_global.D != NULL) {
        // Datos ingresados
        int N = datos_global.N;     // Cantidad de rayos
        int k = datos_global.k;     // Cantidad de datos
        int *D = datos_global.D;    // Vector con los datos
        
        // Configurar color y grosor de las líneas
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_set_line_width(cr, 2);
        // Dibujar los rayos desde el centro
        for (int i = 0; i < k; i++) {
            double angle = (i / (double)N) * 2 * PI;    // Ángulo en radianes
            double length = (D[i] / (double)k) * R;     // Longitud proporcional
            double x_end = xc + length * cos(angle);
            double y_end = yc - length * sin(angle);    // Negativo porque Y crece hacia abajo en GTK
            cairo_move_to(cr, xc, yc);
            cairo_line_to(cr, x_end, y_end);
            cairo_stroke(cr);
        }
    }
    return FALSE;
}

int main(int argc, char *argv[]) {
    GtkBuilder *builder;        // Utilizado para obtener los objetos de glade
    GtkWidget *ventana;         // La ventana
    GtkWidget *area_circulo;    // El área donde se dibuja el círculo
    GtkWidget *panel;           // El panel que divide el área de dibujo y el área de interacción
    GtkWidget *boton_salida;    // Botón para terminar el programa
    GtkWidget *boton_desplegar; // Botón para barajar y mostrar los datos
    GtkWidget *cantidad_rayos;  // Espacio para ingresar cantidad de rayos N
    GtkWidget *cantidad_datos;  // Espacio para ingresar la cantidad de datos k
    GtkWidget *color_1;         // Primer color escogido
    GtkWidget *color_2;         // Segundo color escogido

    srand(time(NULL));  // Para poder utilizar rand() al barajar los números
    gtk_init(&argc, &argv);

    // Cargar la interfaz de Glade
    builder = gtk_builder_new_from_file("interfaz.glade");

    // La ventana
    ventana = GTK_WIDGET(gtk_builder_get_object(builder, "ventana"));
    g_signal_connect(ventana, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // El panel divisor
    panel = GTK_WIDGET(gtk_builder_get_object(builder, "division"));
    g_signal_connect(panel, "notify::position", G_CALLBACK(fijar_panel), NULL);

    // El círculo
    area_circulo = GTK_WIDGET(gtk_builder_get_object(builder, "area_circulo"));
    g_signal_connect(area_circulo, "draw", G_CALLBACK(dibujar_area), NULL);

    // Botón de desplegar los datos del usuario
    boton_desplegar = GTK_WIDGET(gtk_builder_get_object(builder, "boton_desplegar"));
    g_signal_connect(boton_desplegar, "clicked", G_CALLBACK(desplegar_datos), builder);

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
    if (datos_global.D != NULL) {
        free(datos_global.D);
    }

    return 0;
}