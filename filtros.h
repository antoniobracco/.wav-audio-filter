#ifndef FILTROS_H
#define FILTROS_H

#include "audio.h"
#include "buffer.h"

/*Filtro amplificador*/

void filtro_amp(const audio_t* entrada,float amp, audio_t *salida);


/*Filtro normalizador*/

void filtro_norm(const audio_t* entrada, float factor, audio_t *salida);

/*Filtro eco*/

void filtro_eco(const audio_t* entrada, float retardo,float mezcla, audio_t *salida);

/*Filtro overdrive*/

void filtro_over(const audio_t* entrada,float dist, audio_t *salida);


/*Filtro soft clipping*/

void filtro_clip(const audio_t* entrada,float a, audio_t *salida);

/*Filtro chorus*/

void filtro_chorus(const audio_t* entrada, float profundidad_seg, float frecuencia_hz, float mezcla,audio_t *salida);

/*Filtro flanger*/

void filtro_flanger(const audio_t* entrada, float profundidad_seg, float intensidad, float frecuencia_hz, float mezcla,audio_t *salida);

/*Filtro reverb*/

void filtro_reverb(const audio_t* entrada, float profundidad_seg, float intensidad, float mezcla,audio_t *salida);



void hard_clip(audio_t* salida);



#endif











