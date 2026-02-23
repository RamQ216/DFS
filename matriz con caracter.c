#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h> // Necesario para funciones especiales de Windows (Sleep y Cursor)

// Definimos el tamaño del laberinto (siempre impares es mejor)
#define FILAS 21
#define COLS 21

// Variables globales: se pueden usar en cualquier parte del código
char mapa[FILAS][COLS];        // Matriz que guarda los caracteres del laberinto
int camino_f[500];             // Lista para guardar las filas por donde pasa 'E'
int camino_c[500];             // Lista para guardar las columnas por donde pasa 'E'
int total_pasos = 0;           // Contador de cuántos pasos dio la solución

// --- FUNCIONES DE PANTALLA ---

void mover_cursor_al_inicio() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // Obtiene el control de la consola
    COORD pos = {0, 0};                                // Define la coordenada (X=0, Y=0)
    SetConsoleCursorPosition(hConsole, pos);           // Mueve el cursor de escritura al inicio
}

void esconder_cursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // Obtiene el control de la consola
    CONSOLE_CURSOR_INFO info;                          // Estructura para configurar el cursor
    info.dwSize = 100;                                 // Tamaño del cursor
    info.bVisible = FALSE;                             // Cambia la visibilidad a Falso
    SetConsoleCursorInfo(hConsole, &info);             // Aplica la configuración
}

// --- CREACIÓN DEL LABERINTO ---

void llenar_de_paredes() {
    for (int f = 0; f < FILAS; f++) {        // Recorre cada fila
        for (int c = 0; c < COLS; c++) {    // Recorre cada columna
            mapa[f][c] = '#';               // Pone una pared en cada celda
        }
    }
}

void fabricar_laberinto(int f, int c) {
    mapa[f][c] = ' '; // Convierte la pared actual en un camino vacío

    int dir[4] = {0, 1, 2, 3}; // Array con las 4 direcciones posibles
    for (int i = 0; i < 4; i++) {           // Algoritmo para mezclar las direcciones
        int r = rand() % 4;                 // Elige un número al azar del 0 al 3
        int temp = dir[i];                  // Intercambia posiciones (mezcla)
        dir[i] = dir[r];
        dir[r] = temp;
    }

    for (int i = 0; i < 4; i++) {           // Revisa cada dirección mezclada
        int nf = f, nc = c;                 // Nuevas coordenadas temporales
        if (dir[i] == 0) nf = f - 2;        // Intenta saltar 2 espacios arriba
        if (dir[i] == 1) nf = f + 2;        // Intenta saltar 2 espacios abajo
        if (dir[i] == 2) nc = c - 2;        // Intenta saltar 2 espacios izquierda
        if (dir[i] == 3) nc = c + 2;        // Intenta saltar 2 espacios derecha

        // Verifica que no se salga de los bordes del mapa
        if (nf > 0 && nf < FILAS - 1 && nc > 0 && nc < COLS - 1) {
            if (mapa[nf][nc] == '#') {      // Si el destino es una pared cerrada...
                mapa[(f + nf) / 2][(c + nc) / 2] = ' '; // Rompe la pared intermedia
                fabricar_laberinto(nf, nc); // Se llama a sí misma para seguir excavando
            }
        }
    }
}

// --- LÓGICA DE RESOLUCIÓN ---

int visitado[FILAS][COLS]; // Matriz extra para no caminar en círculos

int buscar_ruta(int f, int c) {
    // Caso base: Si llegamos a la salida (esquina inferior derecha)
    if (f == FILAS - 2 && c == COLS - 2) {
        camino_f[total_pasos] = f;          // Guarda la última fila
        camino_c[total_pasos] = c;          // Guarda la última columna
        total_pasos++;                      // Cuenta el último paso
        return 1;                           // Indica que encontró el éxito
    }

    // Si la posición es inválida, hay pared o ya pasamos por aquí, se detiene
    if (f < 0 || f >= FILAS || c < 0 || c >= COLS || mapa[f][c] == '#' || visitado[f][c] == 1) {
        return 0;                           // Indica que este camino no sirve
    }

    visitado[f][c] = 1; // Marca la celda actual como visitada para no volver
    
    // Guarda la posición actual en la lista del camino
    camino_f[total_pasos] = f;
    camino_c[total_pasos] = c;
    total_pasos++;

    // Intenta moverse en las 4 direcciones una por una
    if (buscar_ruta(f + 1, c)) return 1;    // Intenta abajo
    if (buscar_ruta(f - 1, c)) return 1;    // Intenta arriba
    if (buscar_ruta(f, c + 1)) return 1;    // Intenta derecha
    if (buscar_ruta(f, c - 1)) return 1;    // Intenta izquierda

    // Si ninguna dirección funcionó, quita el paso (Backtracking)
    total_pasos--;
    return 0; // Indica fracaso en esta rama
}

// --- FUNCIÓN DE DIBUJO ---

void dibujar(int player_f, int player_c) {
    mover_cursor_al_inicio();                // Mueve el cursor arriba para sobreescribir
    for (int f = 0; f < FILAS; f++) {        // Ciclo para filas
        for (int c = 0; c < COLS; c++) {    // Ciclo para columnas
            if (f == player_f && c == player_c) {
                printf("E ");                // Si es la posición actual, dibuja al personaje
            } else {
                printf("%c ", mapa[f][c]);   // Si no, dibuja el muro o el camino
            }
        }
        printf("\n");                        // Salto de línea al terminar cada fila
    }
}

// --- FUNCIÓN PRINCIPAL ---

int main() {
    srand(time(NULL));      // Inicializa la semilla aleatoria con el reloj actual
    esconder_cursor();      // Quita el guion parpadeante de la pantalla

    // 1. Crear la estructura del laberinto
    llenar_de_paredes();    // Ponemos todo en '#'
    fabricar_laberinto(1, 1); // Empezamos a excavar desde la posición (1,1)
    
    // 2. Colocar las puertas visuales
    mapa[1][0] = '[';       // Símbolo de entrada
    mapa[FILAS - 2][COLS - 1] = ']'; // Símbolo de salida

    // 3. Preparar y ejecutar la búsqueda de la solución
    for(int i=0; i<FILAS; i++) {
        for(int j=0; j<COLS; j++) visitado[i][j] = 0; // Limpia la matriz de visitados
    }
    buscar_ruta(1, 1);      // Encuentra el camino desde el inicio

    // 4. Mostrar la solución paso a paso
    for (int i = 0; i < total_pasos; i++) {
        dibujar(camino_f[i], camino_c[i]); // Dibuja el mapa con la 'E' en el paso actual
        Sleep(20);                         // Pausa de 20 milisegundos para crear animación
    }

    mover_cursor_al_inicio();              // Sube el cursor una vez más
    dibujar(FILAS-2, COLS-2);              // Dibuja el estado final del personaje
    
    printf("\n ¡Llegue al final! \n");      // Mensaje de victoria
    return 0;                               // Termina el programa correctamente
}