// Estructura que guarda los datos que ingresa el usuario
typedef struct {
    int *D;           // Puntero al vector de datos
    int N;            // Cantidad de rayos
    int k;            // Cantidad de datos
    GdkRGBA color_1;  // Primer color elegido
    GdkRGBA color_2;  // Segundo color elegido
} DatosUsuario;
// Estructura que guarda los datos del usuario y los del builder
typedef struct {
    GtkBuilder *builder;
    DatosUsuario *datos;
} AppData;