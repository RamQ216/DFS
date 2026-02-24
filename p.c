#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h> // Solo para Sleep
//definimos las constantes de FILA Y COLUMNA.
#define F 21
#define C 21

char mapa[F][C];//definmos una matriz de tipo caracter co las constantes F Y C

// 1. DIBUJAR (Usamos saltos de línea en lugar de limpiar pantalla)
void dibujar() {
    for (int i = 0; i < F; i++) {
        for (int j = 0; j < C; j++) {
            printf("%c ", mapa[i][j]);
        }
        printf("\n");
    }
}

// 2. CREAR LABERINTO (DFS - El explorador que va al fondo)

void crear_laberinto(int f, int c) {
    mapa[f][c] = ' '; // estamos crando un espacio vacio donde iniciamos
    
    int movimientos[] = {0, 1, 2, 3}; //creamos una lista del 0 al 3 donde cada uno representa un movimiento
    for (int i = 0; i < 4; i++) {
        int mov_aleatorio = rand() % 4;// hacemos que la eleccion de estos elementos sea de manera random
        int movi_temporal = movimientos[i];//guardam
        movimientos[i] = movimientos[mov_aleatorio];
        movimientos[mov_aleatorio] = movi_temporal;
    }

    for (int i = 0; i < 4; i++) {
        int destino_fila = f, destino_columna = c, romperpared_fila = f, romperpared_columna = c;

        if (movimientos[i] == 0) { 
            destino_fila = f - 2; romperpared_fila = f - 1; 
        } // Arriba
        else if (movimientos[i] == 1) { 
            destino_fila = f + 2; romperpared_fila = f + 1; 
        } // Abajo
        else if (movimientos[i] == 2) { 
            destino_columna = c - 2; romperpared_columna = c - 1; 
        } // Izquierda
        else if (movimientos[i] == 3) { 
            destino_columna = c + 2; romperpared_columna = c + 1; 
        } // Derecha

        if (destino_fila > 0 && destino_fila < F - 1 && destino_columna > 0 && destino_columna < C - 1) {
            if (mapa[destino_fila][destino_columna] == '#') {
                mapa[romperpared_fila][romperpared_columna] = ' ';
                crear_laberinto(destino_fila, destino_columna);
            }
        }
    }
}

// 3. RESOLVER (BFS - El que encuentra el camino más corto)
typedef struct { int f, c; } tupla_coordenada;// estamos empaquetando las coordenadas de fila y columna

void resolver_bfs() {
    tupla_coordenada cola[F * C];//reservamos la memoria donde se van a guardar las coordenadas con una estructura de tipo cola
    tupla_coordenada origen[F][C];// este nos sirve para guardar las rutas de donde nacio cada una por asi decirlo
    int visitado[F][C] = {0};// este es nuestro seguro para que no vuelva a lugares por donde ya paso
    int inicio = 0, fin = 0;//definimos las variables necesariar para el bfs

    cola[fin++] = (tupla_coordenada){1, 1};//le designamos el primer valor y luego incrementamos el tamanho de la "lista"
    visitado[1][1] = 1;//aplicamos el seguro hacemos que no vuelva a buscar la primera coordenada

    int encontrado = 0;//bandera para avisar si ya encontro el resultado
    while (inicio < fin) {
        tupla_coordenada actual = cola[inicio++];//aca estamos haciendo que todas las coordenadas sean analizadas

        if (actual.f == F - 2 && actual.c == C - 2) {// aca sera el caso base de neustro bucle
            encontrado = 1;
            break;
        }

        int movimientos_f[] = {-1, 1, 0, 0};//creamos los movientos de arriba y abajo
        int movimientos_columna[] = {0, 0, -1, 1};//creamos los movimientos de izquirda y derecha

        for (int i = 0; i < 4; i++) {//hace los calculos del movimiento
            int destino_fila = actual.f + movimientos_f[i];
            int destino_columna = actual.c + movimientos_columna[i];

            if (mapa[destino_fila][destino_columna] == ' ' && !visitado[destino_fila][destino_columna]) {
                visitado[destino_fila][destino_columna] = 1;//antorcha de minecraft
                origen[destino_fila][destino_columna] = actual;//es el gps te indica que camino recorriste
                cola[fin++] = (tupla_coordenada){destino_fila, destino_columna};//colocamos las coordenadas dentro de neustra ficha y la preparamos para el siguiente dato
            }
        }
    }

    if (encontrado) {
        tupla_coordenada camino = {F - 2, C - 2};
        while (camino.f != 1 || camino.c != 1) {
            mapa[camino.f][camino.c] = 'E'; // creamos la ilusion de que el personaje se mueve
            camino = origen[camino.f][camino.c];//sigue el camini de guardado de fin a inicio
            dibujar();
            printf("--- IA moviéndose por la ruta más corta ---\n");
            Sleep(150);
        }
    }
}

int main() {
    srand(time(NULL));

    // Llenar de paredes
    for (int i = 0; i < F; i++)
        for (int j = 0; j < C; j++)
            mapa[i][j] = '#';

    crear_laberinto(1, 1);
    mapa[1][0] = '[';
    mapa[F - 2][C - 1] = ']';

    dibujar();
    printf("\nGenerado con DFS. BFS calculando linea mas corta");
    Sleep(100);

    resolver_bfs();

    printf("\n¡Ruta optima completada!\n");
    return 0;
}
