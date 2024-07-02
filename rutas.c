#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct punto {
    char letra;
    float x;
    float y;
} punto_t;

float distancia_euclidea(punto_t p1, punto_t p2);
void llena_matriz(punto_t *list, int cant_puntos, float **mat);
void imprime_matriz(int cant_puntos, float **mat);
void estima_ruta(float *costo, punto_t *array, int cant_puntos, float **matriz, char *ruta);

int main() {
    FILE *mi_archivo;
    mi_archivo = fopen("C:\\Users\\ferna\\ClionProjects\\rutas\\rutas_far.txt", "r");
    if (mi_archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        exit(1);
    }

    int cant_puntos, i;
    punto_t *array;
    float **matriz, costo;
    char *ruta;

    fscanf(mi_archivo, "%d\n", &cant_puntos);

    array = malloc(sizeof(punto_t) * cant_puntos);
    if (array == NULL) {
        perror("Error al reservar memoria para array");
        exit(1);
    }

    ruta = malloc(sizeof(char) * (cant_puntos + 1));
    if (ruta == NULL) {
        perror("Error al reservar memoria para ruta");
        exit(1);
    }

    matriz = (float **)malloc(sizeof(float *) * cant_puntos);
    if (matriz == NULL) {
        perror("Error al reservar memoria para matriz");
        exit(1);
    }
    for (i = 0; i < cant_puntos; i++) {
        matriz[i] = (float *)malloc(sizeof(float) * cant_puntos);
        if (matriz[i] == NULL) {
            perror("Error al reservar memoria para matriz[i]");
            exit(1);
        }
    }
    for (i = 0; i < cant_puntos; i++) {
        fscanf(mi_archivo, "%c,%f,%f\n", &array[i].letra, &array[i].x, &array[i].y);
        printf("%c,%.14f,%.14f\n", array[i].letra, array[i].x, array[i].y);
        fflush(stdin);
    }

    llena_matriz(array, cant_puntos, matriz);
    imprime_matriz(cant_puntos, matriz);

    costo = 0;
    estima_ruta(&costo, array, cant_puntos, matriz, ruta);
    printf("La mejor ruta es %s con un costo de %g\n", ruta, costo);

    for (i = 0; i < cant_puntos; i++) {
        free(matriz[i]);
    }
    free(matriz);
    free(array);
    free(ruta);
    fclose(mi_archivo);
    return 0;
}

float distancia_euclidea(punto_t p1, punto_t p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

void llena_matriz(punto_t *list, int cant_puntos, float **mat) {
    int i, j;
    float distancia;
    for (i = 0; i < cant_puntos; i++) {
        for (j = 0; j <= i; j++) {
            if (i == j) {
                mat[i][j] = -1;
            } else {
                distancia = distancia_euclidea(list[i], list[j]);
                mat[i][j] = distancia;
                mat[j][i] = distancia;
            }
        }
    }
}

void imprime_matriz(int cant_puntos, float **mat) {
    int i, j;
    for (i = 0; i < cant_puntos; i++) {
        for (j = 0; j < cant_puntos; j++) {
            // Imprimir con 14 decimales
            printf("%0.2f\t", mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void estima_ruta(float *costo, punto_t *array, int cant_puntos, float **matriz, char *ruta) {
    int i, j, inicio, actual, siguiente;
    float distancia_minima;

    *costo = 0;
    inicio = 0;

    int visitado[cant_puntos];
    for (i = 0; i < cant_puntos; i++) {
        visitado[i] = 0;
    }

    actual = inicio;
    ruta[0] = array[inicio].letra;
    visitado[inicio] = 1;

    for (i = 1; i < cant_puntos; i++) {
        distancia_minima = INFINITY;
        siguiente = -1;

        for (j = 0; j < cant_puntos; j++) {
            if (!visitado[j] && matriz[actual][j] < distancia_minima) {
                distancia_minima = matriz[actual][j];
                siguiente = j;
            }
        }

        *costo += distancia_minima;

        visitado[siguiente] = 1;
        actual = siguiente;
        ruta[i] = array[siguiente].letra;
    }

    *costo += matriz[actual][inicio];
    ruta[cant_puntos] = array[inicio].letra;
    ruta[cant_puntos + 1] = '\0';
}
