/* audio.h */
#ifndef AUDIO_H
#define AUDIO_H


#define MAX_VAL             32767
#define MIN_VAL             -32768

typedef enum wav_resultado_t  {
	WAV_OK,
	WAV_ERROR_LECTURA,
	WAV_ENCABEZADO_INVALIDO,
	WAV_ERROR_ESCRITURA
}wav_resultado_t;

typedef enum canales_t  {
	MONO,
	STEREO
}canales_t;

typedef struct audio_t {
	enum canales_t canales;
	unsigned int frec_muestreo;
	unsigned int largo;
	float* muestras;
}audio_t;


void inicializar_audio(audio_t* audio);

void destruir_audio(audio_t *audio);

wav_resultado_t leer_wav(const char* ruta, audio_t *audio);

wav_resultado_t escribir_wav(const char* ruta,audio_t *audio);

int bytesaentero(char bytes[4]);

short bytesashort(char bytes[2]);

#endif


