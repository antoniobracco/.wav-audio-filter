#ifndef BUFFER_H
#define BUFFER_H


#define BUFFER_SIZE 8192
#define BUFFER_MASK 8191

typedef struct buffer_t {
	unsigned int posicion;
	float* almacenamiento;
}buffer_t;

void inicializar_buffer(buffer_t *buffer);

float leer_buffer(float retraso, buffer_t *buffer);

void escribir_buffer(float muestra, buffer_t *buffer);

void destruir_buffer(buffer_t *buffer);
	
#endif
