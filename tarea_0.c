/*
                Tarea 0: Rayos Desordenados
                Hecha por: Carmen Hidalgo Paz, Jorge Guevara Chavarría y Ricardo Castro Jiménez
                Fecha: Jueves 27 de febrero del 2025

                Esta sección contiene el main, donde se indica lo que tiene que hacer
                cada objeto mostrado en la interfaz. Además, hay una función que muestra los
                rayos y el círculo editado con los valores que ingresa el usuario.
                Estas ediciones del usuario son guardadas en otra función llamada desplegar_datos.
                El resto de funciones barajan los datos del vector D o afectan la visualización
                de la interfaz.
*/

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
    DatosGenerales *general = (DatosGenerales *)user_data;
    GtkBuilder *builder = general->builder;
    DatosUsuario *datos = general->datos;

    // Número de rayos ingresados por el usuario
    GtkWidget *cantidad_rayos = GTK_WIDGET(gtk_builder_get_object(builder, "cantidad_rayos"));
    int N = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(cantidad_rayos));
    // Cantidad de datos ingresados por el usuario
    GtkWidget *cantidad_datos = GTK_WIDGET(gtk_builder_get_object(builder, "cantidad_datos"));
    int k = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(cantidad_datos));
    // Se guarda el primer color escogido
    GdkRGBA color_1;
    GtkWidget *primer_color = GTK_WIDGET(gtk_builder_get_object(builder, "color_1"));
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(primer_color), &color_1);
    // Se guarda el segundo color escogido
    GdkRGBA color_2;
    GtkWidget *segundo_color = GTK_WIDGET(gtk_builder_get_object(builder, "color_2"));
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(segundo_color), &color_2);

    // Datos del usuario
    datos->N = N;
    datos->k = k;
    datos->color_1 = color_1;
    datos->color_2 = color_2;
    
    // Limpiar memoria si ya había un vector
    if (datos->D != NULL) {
        free(datos->D);
    }
    // Crear un vector con k espacios en memoria dinámica
    datos->D = malloc(sizeof(int) * k);
    // Si el número ingresado es muy grande
    if (datos->D == NULL) {
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
        "No se puede asignar memoria para esta cantidad de dígitos. Por favor ingrese un valor más pequeño.");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    return;  // El usuario puede volver a intentar
}
    for (int i = 0; i < k; i++) {
        datos->D[i] = i + 1;  // Llenar el vector con datos desde 1 hasta k
    }
    barajar_datos(datos->D, k);   // Cambiar de orden los valores en el vector
    
    // Volver a dibujar el círculo, esta vez con los rayos
    GtkWidget *area_circulo = GTK_WIDGET(gtk_builder_get_object(builder, "area_circulo"));
    gtk_widget_queue_draw(area_circulo);
}
// Crea la lista de colores que va a corresponder a los rayos
void colorLinea(int *D, int cElementosV, int color1[3], int color2[3], int colores[][3]) {
	for (int i = 0; i < cElementosV; i++) {
		for (int j = 0; j < 3; j++) {
			colores[i][j] = color1[j] + ((D[i] - 1) * (color2[j] - color1[j])) / (cElementosV - 1);
		}
	}
}
// Mostrar círculo y rayos
gboolean dibujar_area(GtkWidget *area, cairo_t *cr, gpointer user_data) {
    DatosUsuario *datos = (DatosUsuario *)user_data;
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
    if (datos->D != NULL) {
        // Datos ingresados
        int N = datos->N;     // Cantidad de rayos
        int k = datos->k;     // Cantidad de datos
        int *D = datos->D;    // Vector con los datos
        
        // Configurar grosor de las líneas
        cairo_set_line_width(cr, 5);

        // Pasar de RBGA a RGB
        int color1[3] = {
            (int)(datos->color_1.red * 255),
            (int)(datos->color_1.green * 255),
            (int)(datos->color_1.blue * 255)
        };
        int color2[3] = {
            (int)(datos->color_2.red * 255),
            (int)(datos->color_2.green * 255),
            (int)(datos->color_2.blue * 255)
        };
        // Arreglo con los colores para todas los rayos
        int colores[datos->k][3];
        // Función que llena el arreglo con colores
        colorLinea(datos->D, datos->k, color1, color2, colores);
        // Dibujar los rayos desde el centro
        for (int i = 0; i < k; i++) {
            double angle = (i / (double)N) * 2 * PI;    // Ángulo en radianes
            double length = (D[i] / (double)k) * R;     // Longitud proporcional
            double x_end = xc + length * cos(angle);
            double y_end = yc - length * sin(angle);    // Negativo porque Y crece hacia abajo en GTK
            
            // Convierte los valores del RGB para que cairo los pueda utilizar
            double r = colores[i][0] / 255.0;
            double g = colores[i][1] / 255.0;
            double b = colores[i][2] / 255.0;
            
            cairo_set_source_rgb(cr, r, g, b);
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

    // Se asigna memoria para la estructura de DatosUsuario
    DatosUsuario *datos = malloc(sizeof(DatosUsuario));
    if (!datos) {
        fprintf(stderr, "No se pudo asignar memoria dinámica.\n");
        return EXIT_FAILURE;
    }
    // Se guardan valores en las variables
    datos->D = NULL;
    datos->N = 0;
    datos->k = 0;
    datos->color_1 = (GdkRGBA){0, 0, 0, 1};
    datos->color_2 = (GdkRGBA){0, 0, 0, 1};

    // Se asigna memoria para la estructura de DatosGenerales
    DatosGenerales *general = malloc(sizeof(DatosGenerales));
    if (!general) {
        fprintf(stderr, "No se pudo asignar memoria dinámica.\n");
        free(datos);
        return EXIT_FAILURE;
    }
    general->builder = builder;
    general->datos = datos;

    // La ventana
    ventana = GTK_WIDGET(gtk_builder_get_object(builder, "ventana"));
    g_signal_connect(ventana, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    // El panel divisor
    panel = GTK_WIDGET(gtk_builder_get_object(builder, "division"));
    g_signal_connect(panel, "notify::position", G_CALLBACK(fijar_panel), NULL);
    // El círculo
    area_circulo = GTK_WIDGET(gtk_builder_get_object(builder, "area_circulo"));
    g_signal_connect(area_circulo, "draw", G_CALLBACK(dibujar_area), datos);
    // Botón de desplegar los datos del usuario
    boton_desplegar = GTK_WIDGET(gtk_builder_get_object(builder, "boton_desplegar"));
    g_signal_connect(boton_desplegar, "clicked", G_CALLBACK(desplegar_datos), general);
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
    if (datos->D != NULL) {
        free(datos->D);
    }
    free(datos);
    free(general);
    g_object_unref(builder);

    return 0;
}