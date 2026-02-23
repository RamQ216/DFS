#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h> // Solo para Sleep

#define F 21
#define C 21

char mapa[F][C];

// 1. DIBUJAR (Usamos saltos de línea en lugar de limpiar pantalla)
void dibujar() {
    // Imprimimos muchos saltos de línea para "limpiar" visualmente
    for (int s = 0; s < 10; s++) printf("\n"); 

    for (int i = 0; i < F; i++) {
        for (int j = 0; j < C; j++) {
            printf("%c ", mapa[i][j]);
        }
        printf("\n");
    }
}

// 2. CREAR LABERINTO (DFS - El explorador que va al fondo)
void crear_laberinto(int f, int c) {
    mapa[f][c] = ' '; 
    
    int dir[] = {0, 1, 2, 3}; 
    for (int i = 0; i < 4; i++) {
        int r = rand() % 4;
        int temp = dir[i];
        dir[i] = dir[r];
        dir[r] = temp;
    }

    for (int i = 0; i < 4; i++) {
        int nf = f, nc = c, pf = f, pc = c;

        if (dir[i] == 0) { nf = f - 2; pf = f - 1; } // Arriba
        else if (dir[i] == 1) { nf = f + 2; pf = f + 1; } // Abajo
        else if (dir[i] == 2) { nc = c - 2; pc = c - 1; } // Izquierda
        else if (dir[i] == 3) { nc = c + 2; pc = c + 1; } // Derecha

        if (nf > 0 && nf < F - 1 && nc > 0 && nc < C - 1) {
            if (mapa[nf][nc] == '#') {
                mapa[pf][pc] = ' ';
                crear_laberinto(nf, nc);
            }
        }
    }
}

// 3. RESOLVER (BFS - El que encuentra el camino más corto)
typedef struct { int f, c; } Punto;

void resolver_bfs() {
    Punto cola[F * C];
    Punto padre[F][C];
    int visitado[F][C] = {0};
    int inicio = 0, fin = 0;

    cola[fin++] = (Punto){1, 1};
    visitado[1][1] = 1;

    int encontrado = 0;
    while (inicio < fin) {
        Punto actual = cola[inicio++];

        if (actual.f == F - 2 && actual.c == C - 2) {
            encontrado = 1;
            break;
        }

        int df[] = {-1, 1, 0, 0};
        int dc[] = {0, 0, -1, 1};

        for (int i = 0; i < 4; i++) {
            int nf = actual.f + df[i];
            int nc = actual.c + dc[i];

            if (mapa[nf][nc] == ' ' && !visitado[nf][nc]) {
                visitado[nf][nc] = 1;
                padre[nf][nc] = actual;
                cola[fin++] = (Punto){nf, nc};
            }
        }
    }

    if (encontrado) {
        Punto camino = {F - 2, C - 2};
        while (camino.f != 1 || camino.c != 1) {
            mapa[camino.f][camino.c] = 'E'; // Personaje moviéndose
            camino = padre[camino.f][camino.c];
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
    printf("\nGenerado con DFS. Pulsa ENTER para ver BFS...");
    getchar();

    resolver_bfs();

    printf("\n¡Ruta optima completada!\n");
    return 0;
}
