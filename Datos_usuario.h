// Estructura que guarda los datos que ingresa el usuario
typedef struct {
    int *D;           // Puntero al vector de datos
    int N;            // Cantidad de rayos
    int k;            // Cantidad de datos
    GdkRGBA color_1;  // Primer color elegido
    GdkRGBA color_2;  // Segundo color elegido
} DatosUsuario;
// Hacer la estructura global para poder cambiar los datos
DatosUsuario datos_global = { NULL, 0, 0, {0, 0, 0, 1}, {0, 0, 0, 1} };