// Estructura que guarda los datos que ingresa el usuario
typedef struct {
    int *D;     // Puntero al vector de datos
    int N;      // Cantidad de rayos
    int k;      // Cantidad de datos
} DatosUsuario;
DatosUsuario datos_global = { NULL, 0, 0 }; // Hacer la estructura global para
                                            // poder cambiar los datos