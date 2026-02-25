#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>


// 1. Cambiamos las constantes por variables globales para que sean dinámicas
int F = 21; 
int C = 21;
#define MAX_F 100 // Límite máximo de la matriz estática
#define MAX_C 100

char mapa[MAX_F][MAX_C]; 

// --- TUS FUNCIONES SE MANTENDRÁN IGUAL (usarán los valores de F y C globales) ---

void dibujar() {
    for (int i = 0; i < F; i++) {
        for (int j = 0; j < C; j++) {
            printf("%c ", mapa[i][j]);
        }
        printf("\n");
    }
}

void crear_laberinto(int f, int c) {
    mapa[f][c] = ' '; 
    int movimientos[] = {0, 1, 2, 3}; 
    for (int i = 0; i < 4; i++) {
        int mov_aleatorio = rand() % 4;
        int movi_temporal = movimientos[i];
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

typedef struct { int f, c; } tupla_coordenada;

void resolver_bfs() {
    // Usamos punteros o arreglos basados en el máximo para la cola
    tupla_coordenada camino[MAX_F][MAX_C];
    tupla_coordenada *cola = malloc(F * C * sizeof(tupla_coordenada));
    tupla_coordenada origen[MAX_F][MAX_C];
    int visitado[MAX_F][MAX_C] = {0};
    int inicio = 0, fin = 0;

    cola[fin++] = (tupla_coordenada){1, 1};
    visitado[1][1] = 1;

    int encontrado = 0;
    while (inicio < fin) {
        tupla_coordenada actual = cola[inicio++];

        if (actual.f == F - 2 && actual.c == C - 2) {
            encontrado = 1;
            break;
        }

        int movimientos_f[] = {-1, 1, 0, 0};
        int movimientos_columna[] = {0, 0, -1, 1};

        for (int i = 0; i < 4; i++) {
            int d_f = actual.f + movimientos_f[i];
            int d_c = actual.c + movimientos_columna[i];

            if (d_f >= 0 && d_f < F && d_c >= 0 && d_c < C && mapa[d_f][d_c] == ' ' && !visitado[d_f][d_c]) {
                visitado[d_f][d_c] = 1;
                origen[d_f][d_c] = actual;
                cola[fin++] = (tupla_coordenada){d_f, d_c};
            }
        }
    }

   if (encontrado) {
    // 1. Creamos un arreglo para guardar el camino (máximo F*C pasos)
    tupla_coordenada pasos[MAX_F * MAX_C];
    int total_pasos = 0;
    
    tupla_coordenada actual = {F - 2, C - 2};

    // 2. Retrocedemos desde el final al inicio guardando los puntos
    while (actual.f != 1 || actual.c != 1) {
        pasos[total_pasos++] = actual;
        actual = origen[actual.f][actual.c];
    }
    // Agregamos el punto de inicio (opcional, si quieres marcarlo)
    pasos[total_pasos++] = (tupla_coordenada){1, 1};

    // 3. Recorremos el arreglo de ATRÁS hacia ADELANTE
    for (int i = total_pasos - 1; i >= 0; i--) {
        tupla_coordenada p = pasos[i];
        
        mapa[p.f][p.c] = 'E'; // Marcamos el camino
        
        dibujar();
        printf("--- IA moviéndose por la ruta más corta (Inicio -> Fin) ---\n");
        Sleep(150);
        }
    }
}

// --- MAIN IMPLEMENTANDO TU SUGERENCIA ---

int main(int argc, char* argv[]) {
    srand(time(NULL));

    // 1. Procesar argumentos de consola
    if (argc == 3) {
        int filas_input = atoi(argv[1]);
        int cols_input = atoi(argv[2]);

        // VALIDACIÓN: El tamaño mínimo lógico para este algoritmo es 5x5
        // Y el máximo es el límite de la matriz (MAX_F/MAX_C)
        if (filas_input >= 5 && filas_input < MAX_F && cols_input >= 5 && cols_input < MAX_C) {
            F = filas_input;
            C = cols_input;
        } else {
            printf("Error: Dimensiones invalidas. Se usara el tamano por defecto (21x21).\n");
            printf("El rango permitido es de 5 a %d.\n", MAX_F - 1);
            F = 21;
            C = 21;
        }

        // SUGERENCIA DE PARIDAD: Ajustar a números impares
        // Si el usuario pone 10, lo bajamos a 9 para que el borde encaje
        if (F % 2 == 0) F--;
        if (C % 2 == 0) C--;
    }

    // 2. Llenar de paredes (usando los valores finales de F y C)
    for (int i = 0; i < F; i++) {
        for (int j = 0; j < C; j++) {
            mapa[i][j] = '#';
        }
    }

    // 3. Iniciar lógica del laberinto
    crear_laberinto(1, 1);
    
    // Colocar entrada y salida dinámicamente
    mapa[1][0] = '[';
    mapa[F - 2][C - 1] = ']';

    dibujar();
    printf("\nConfiguracion final: %dx%d", F, C);
    printf("\nGenerado con DFS. BFS calculando ruta...");
    Sleep(300);

    resolver_bfs();

    printf("\n¡Ruta optima completada!\n");
    return 0;
}