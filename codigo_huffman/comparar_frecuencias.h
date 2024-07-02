#ifndef COMPARAR_FRECUENCIAS_H
#define COMPARAR_FRECUENCIAS_H
#include <stdio.h>

typedef struct {
    char caracter;
    int frecuencia;
} CaracterConFrecuencia;

int comparar_frecuencias(const void* a, const void* b) {
    CaracterConFrecuencia* elemA = (CaracterConFrecuencia*)a;
    CaracterConFrecuencia* elemB = (CaracterConFrecuencia*)b;

    if (elemA->frecuencia > elemB->frecuencia) {
        return 1;
    } else if (elemA->frecuencia < elemB->frecuencia) {
        return -1;
    } else {
        return 0;
}

#endif
