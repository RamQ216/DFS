#include <stdio.h>// necesario para la entrada y salida de datos
#include <stdlib.h>
#include <time.h>
#include <windows.h>//para el sleep


//definimos variables globales
int F = 21; 
int C = 21;
//definimos constantes que funcionan como espacio para la memoria
#define MAX_F 100 
#define MAX_C 100
//definimos una mtriz de tipo caracter
char mapa[MAX_F][MAX_C]; 


//creamos la matriz
void matriz() {
    for (int i = 0; i < F; i++) {
        for (int j = 0; j < C; j++) {
            printf("%c ", mapa[i][j]);// indicamos que vamos a recibir un dato de tipo caracter
        }
        printf("\n");
    }
}
//DFS
void crear_laberinto(int f, int c) {
    mapa[f][c] = ' ';//creamos un espacio en la pos_actual 
    int movimientos[] = {0, 1, 2, 3}; //creamos una lista con 4 mov posibles
    for (int i = 0; i < 4; i++) {
        int mov_aleatorio = rand() % 4;//creamos una vraible que guarda numeros random del 1 al 4
        int movi_temporal = movimientos[i];//colocamos el mov dentro de un aux
        movimientos[i] = movimientos[mov_aleatorio];//guardamos el numero aleatorio dentro de el mov
        movimientos[mov_aleatorio] = movi_temporal;//guardamos el aux dentro de el mov aleatorio
    }

    for (int i = 0; i < 4; i++) {
        int destino_fila = f, destino_columna = c, romperpared_fila = f, romperpared_columna = c;//definimos variables 

        if (movimientos[i] == 0) { 
            destino_fila = f - 2; romperpared_fila = f - 1; 
        } // romper pared de Arriba
        else if (movimientos[i] == 1) { 
            destino_fila = f + 2; romperpared_fila = f + 1; 
        } // romper pared de Abajo
        else if (movimientos[i] == 2) { 
            destino_columna = c - 2; romperpared_columna = c - 1; 
        } // romper pared de la Izquierda
        else if (movimientos[i] == 3) { 
            destino_columna = c + 2; romperpared_columna = c + 1; 
        } // romper pared de la Derecha

        if (destino_fila > 0 && destino_fila < F - 1 && destino_columna > 0 && destino_columna < C - 1) {//definier limites
            if (mapa[destino_fila][destino_columna] == '#') {//condicional para que solo rompa lo que todavia no rompio
                mapa[romperpared_fila][romperpared_columna] = ' ';// crea el camino o mejor dicho rompe un pared
                crear_laberinto(destino_fila, destino_columna);//se llama asi misma para crear el laberinto ya hecho
            }
        }
    }
}

typedef struct { int f, c; } tupla_coordenada;//creamos una ficha o una variable capaz de guardar mas de un elemento

void resolver_bfs() {
    tupla_coordenada camino[MAX_F][MAX_C];//lista donde se guardan las coordenadas para recorrer de manera correcta 
    tupla_coordenada *cola = malloc(F * C * sizeof(tupla_coordenada)); // Usamos punteros o arreglos basados en el máximo para la cola
    tupla_coordenada origen[MAX_F][MAX_C];//creamos una lista donde se guarda el camino recorrido
    int visitado[MAX_F][MAX_C] = {0};//inicializamos en cero para no tener cache
    int inicio = 0, fin = 0;//creamos variables para neustra cola

    cola[fin++] = (tupla_coordenada){1, 1};//guardamos la primer coordenada
    visitado[1][1] = 1;//indicamos que la primer pos ya fue explorada 

    int encontrado = 0;//bandera
    while (inicio < fin) {//necesario para recorrer toda la lista
        tupla_coordenada pos_actual = cola[inicio++];//guardamos la primera pos como la pos_actual y pasamos a la sig

        if (pos_actual.f == F - 2 && pos_actual.c == C - 2) {//  levantar bandera
            encontrado = 1;
            break;
        }

        int movimientos_f[] = {-1, 1, 0, 0};//creamos mov de fila
        int movimientos_columna[] = {0, 0, -1, 1};//creamos mov de columna

        for (int i = 0; i < 4; i++) {
            int d_f = pos_actual.f + movimientos_f[i];//hacemos que el presonaje camie su pos de fila
            int d_c = pos_actual.c + movimientos_columna[i];//hacemos que el presonaje camie su pos de columna

            if (d_f >= 0 && d_f < F && d_c >= 0 && d_c < C && mapa[d_f][d_c] == ' ' && !visitado[d_f][d_c]) {//condicional para no salir del m
                visitado[d_f][d_c] = 1;//marcamos los caminos recorridos
                origen[d_f][d_c] = pos_actual;//cambiamos de posoicion
                cola[fin++] = (tupla_coordenada){d_f, d_c};//guardamos en la cola las coordenadas ya vista
            }
        }
    }

   if (encontrado) {

    tupla_coordenada pasos[MAX_F * MAX_C];//creamos el espacio de memoria
    int total_pasos = 0;//creamos el contador
    
    tupla_coordenada actual = {F - 2, C - 2};// posicion final o origen

    // 2. Retrocedemos desde el final al inicio guardando los puntos
    while (actual.f != 1 || actual.c != 1) {//aca hacemos el backtrakind
        pasos[total_pasos++] = actual;//guardamos las posiciones ya recorridas
        actual = origen[actual.f][actual.c];//guardamos el camino recorrido paso a paso
    }
    
    pasos[total_pasos++] = (tupla_coordenada){1, 1};//puto de inicio

    
    for (int i = total_pasos - 1; i >= 0; i--) {//recorremos el bactrakind de manera inversa
        tupla_coordenada p = pasos[i];//recorremos la lista de las coordenadas en orden de inicio a fin
        
        mapa[p.f][p.c] = 'E'; // Marcamos el camino co las E
        
        matriz();// volvemos a imprimir
        printf("RECORRIENDO LABERINTO\n");
        Sleep(150);//intervalo de tiempo
        }
    }
}



int main(int argc, char* argv[]) {//parametros necesario para cambiar la matriz desde terminal
    srand(time(NULL));//hace que sea aleatorio de verdad

    // 1. Procesar argumentos de consola
    if (argc == 3) {//cantidad de palabras escritas por usuario
        int filas_input = atoi(argv[1]);//transforma el caracter del numero e un entero
        int cols_input = atoi(argv[2]);//transforma el caracter del numero e un entero

        //hace que el codigo tenga un minimo de 5x5 y tambien que no supere el tamanho de memoria elegido
        if (filas_input >= 5 && filas_input < MAX_F && cols_input >= 5 && cols_input < MAX_C) {
            F = filas_input;
            C = cols_input;
        } else {// si es que no se cumplen las condicionales se ejecuta en 21x21
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

    //crea el tablero llenito de paredes
    for (int i = 0; i < F; i++) {
        for (int j = 0; j < C; j++) {
            mapa[i][j] = '#';
        }
    }

    // le indicamos  los valores iniciales
    crear_laberinto(1, 1);
    
    // Colocar entrada y salida
    mapa[1][0] = '[';
    mapa[F - 2][C - 1] = ']';

    matriz();
    printf("\nDimensiones: %dx%d", F, C);
    printf("\ncalculando ruta optima");
    Sleep(200);

    resolver_bfs();

    printf("\n¡Ruta optima completada!\n");
    return 0;
}
