#ifndef BLOQUES_H
#define BLOQUES_H
#include "buffer.h"


float bloque_lut(float x, float T[1024]);

float s(double fase, float a, float frec);

float t(double fase, float a, float frec);

float bloque_lfo(int tipo, float f0, float a, double* ppsi, unsigned frec_muestreo);

float bloque_eco(float muestra, float ret, float mezcla, buffer_t *buffer);

float bloque_comb (float muestra, float ret, float intensidad, buffer_t *buffer);




#endif
