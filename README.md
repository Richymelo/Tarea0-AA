# Tarea0-AA

Descripción:
Este programa dibuja lineas desordenadas que salen del centro de un círculo, usando colores y longitudes variables. Se utiliza GTK y Glade para la interfaz gráfica y Cairo para el renderizado. Los rayos se generan con base en dos colores seleccionados por el usuario y un conjunto de valores aleatorios.  

Funcionalidad:
1. El usuario ingresa:  
   - `N` → Número de rayos (mínimo 4).  
   - `k` → Número de datos en el vector (mínimo 4).  
   - `C0` y `C1` → Dos colores RGB base.  
2. Se genera un vector `D` de tamaño `k` con valores del 1 a `k`, y se baraja aleatoriamente.  
3. Cada vez que el usuario presiona el botón de despliegue: 
   - Se vuelve a barajar el vector `D`.  
   - Se dibujan `N` líneas con:  
     - **Ángulo**: \( \frac{i}{N} \times 2\pi \) radianes.  
     - **Longitud**: \( \frac{D[i]}{k} \times R \).  
     - **Color**: Interpolación entre `C0` y `C1`.  
4. El botón "Terminar" cierra el programa.

5. Requisitos:
6. Sistema operativo: Linux.  
- Librerías necesarias: 
  - GTK  
  - Cairo  
  - Glade   
- Compilador: `gcc` 
