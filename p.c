#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h> 

#define FILAS 21 
#define COLUMNAS 21

char laberinto[FILAS][COLUMNAS];

// --- FUNCIONES SIMPLES ---

void crear_bloque_solido() {
    int i, j;
    for (i = 0; i < FILAS; i++) {
        for (j = 0; j < COLUMNAS; j++) {
            laberinto[i][j] = '#';
        }
    }
}

void mostrar_laberinto() {
    system("cls"); 
    int i, j;
    for (i = 0; i < FILAS; i++) {
        for (j = 0; j < COLUMNAS; j++) {
            printf("%c ", laberinto[i][j]);
        }
        printf("\n");
    }
}

// Generación paso a paso (sin bucles raros)
void generar_dfs(int fila, int columna) {
    laberinto[fila][columna] = ' ';

    int opciones[4] = {0, 1, 2, 3}; // 0:Arriba, 1:Abajo, 2:Izq, 3:Der
    
    // Mezclar direcciones manualmente
    int i;
    for (i = 0; i < 4; i++) {
        int r = rand() % 4;
        int temporal = opciones[i];
        opciones[i] = opciones[r];
        opciones[r] = temporal;
    }

    for (i = 0; i < 4; i++) {
        // Intentar Arriba
        if (opciones[i] == 0) {
            if (fila - 2 > 0 && laberinto[fila - 2][columna] == '#') {
                laberinto[fila - 1][columna] = ' ';
                generar_dfs(fila - 2, columna);
            }
        }
        // Intentar Abajo
        else if (opciones[i] == 1) {
            if (fila + 2 < FILAS - 1 && laberinto[fila + 2][columna] == '#') {
                laberinto[fila + 1][columna] = ' ';
                generar_dfs(fila + 2, columna);
            }
        }
        // Intentar Izquierda
        else if (opciones[i] == 2) {
            if (columna - 2 > 0 && laberinto[fila][columna - 2] == '#') {
                laberinto[fila][columna - 1] = ' ';
                generar_dfs(fila, columna - 2);
            }
        }
        // Intentar Derecha
        else if (opciones[i] == 3) {
            if (columna + 2 < COLUMNAS - 1 && laberinto[fila][columna + 2] == '#') {
                laberinto[fila][columna + 1] = ' ';
                generar_dfs(fila, columna + 2);
            }
        }
    }
}

void colocar_entrada_y_salida() {
    laberinto[1][0] = '[';
    laberinto[FILAS - 2][COLUMNAS - 1] = ']';
}

// --- RESOLUCIÓN PASO A PASO (Súper explícita) ---

int resolver_ia(int fila_actual, int columna_actual) {
    
    // 1. ¿Llegamos a la meta?
    if (laberinto[fila_actual][columna_actual] == ']') {
        return 1;
    }

    // 2. Dejar rastro del personaje
    laberinto[fila_actual][columna_actual] = 'E';
    mostrar_laberinto();
    printf("\n Estoy en la fila %d y columna %d", fila_actual, columna_actual);
    Sleep(50);

    // 3. Probar cada camino por separado (muy manual)

    // PROBAR ARRIBA
    int prox_f = fila_actual - 1;
    int prox_c = columna_actual;
    if (prox_f >= 0 && (laberinto[prox_f][prox_c] == ' ' || laberinto[prox_f][prox_c] == ']')) {
        if (resolver_ia(prox_f, prox_c) == 1) return 1;
    }

    // PROBAR ABAJO
    prox_f = fila_actual + 1;
    prox_c = columna_actual;
    if (prox_f < FILAS && (laberinto[prox_f][prox_c] == ' ' || laberinto[prox_f][prox_c] == ']')) {
        if (resolver_ia(prox_f, prox_c) == 1) return 1;
    }

    // PROBAR IZQUIERDA
    prox_f = fila_actual;
    prox_c = columna_actual - 1;
    if (prox_c >= 0 && (laberinto[prox_f][prox_c] == ' ' || laberinto[prox_f][prox_c] == ']')) {
        if (resolver_ia(prox_f, prox_c) == 1) return 1;
    }

    // PROBAR DERECHA
    prox_f = fila_actual;
    prox_c = columna_actual + 1;
    if (prox_c < COLUMNAS && (laberinto[prox_f][prox_c] == ' ' || laberinto[prox_f][prox_c] == ']')) {
        if (resolver_ia(prox_f, prox_c) == 1) return 1;
    }

    // 4. Si llegamos aquí, este camino no sirve
    laberinto[fila_actual][columna_actual] = 'v'; // 'v' de visitado/vacio
    return 0;
}

int main() {
    srand(time(NULL));
    
    crear_bloque_solido();
    generar_dfs(1, 1);
    colocar_entrada_y_salida();

    mostrar_laberinto();
    printf("\n Presiona Enter para que la IA empiece...");
    getchar();

    // Empezar desde la entrada
    if (resolver_ia(1, 0) == 1) {
        mostrar_laberinto();
        printf("\n ¡El personaje llego a la salida solo!\n");
    } else {
        printf("\n No se encontro un camino.\n");
    }

    return 0;
}