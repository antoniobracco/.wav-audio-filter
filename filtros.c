//FILTROS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "bloques.h"
#include "audio.h"
#include "buffer.h"

void hard_clip(audio_t* salida) {
	int i=0;
	int cantidadmuestras=(*salida).largo*((*salida).canales+1);
	float* s=(*salida).muestras;
	for(i=0;i<cantidadmuestras;i++) {
	
		if((*s)<MIN_VAL){

			*s=MIN_VAL;
		}
		else if((*s)>MAX_VAL){

			*s=MAX_VAL;
		}
		s++;
	}
}


void filtro_amp(const audio_t* entrada,float amp, audio_t* salida){
	float* p=(*entrada).muestras;
	float* s=(*salida).muestras;
	int cantidadmuestras=(*entrada).largo*((*entrada).canales+1);
	int i;
	for(i=0;i<cantidadmuestras;i++) {
	
		if(amp*(*p)<MIN_VAL){

			*s=MIN_VAL;
		}
		else if(amp*(*p)>MAX_VAL){

			*s=MAX_VAL;
		}
		else{

			*s=(amp*(*p));
			
		}
	s++;
	p++;
	}
}





void filtro_over(const audio_t* entrada,float dist, audio_t* salida){
	int i=0;
	int largomuestras=((*entrada).largo)*((*entrada).canales+1);
	float* p=(*entrada).muestras;
	float* s=(*salida).muestras;

	float T[1024];// Inicializo una tabla LUT
	int x;
	for(i=0;i<1024;i++) {
		x=-MAX_VAL-1+i*64;

		if(x<0){			// si sign(x)=-1

		T[i]=-powf(powf(MAX_VAL,dist)-powf(abs(abs(x)-MAX_VAL),dist),1/dist);
		}
		else{

		T[i]=powf(powf(MAX_VAL,dist)-powf(abs(abs(x)-MAX_VAL),dist),1/dist);
		}

	}	

	for(i=0;i<largomuestras;i++){

		*s=(bloque_lut(*p,T));// Utilizo el bloque LUT con la tabla inicializada
		s++;
		p++;
	}
	hard_clip(salida);	

}



void filtro_clip(const audio_t* entrada,float a, audio_t* salida){
	int i=0;
	int largomuestras=((*entrada).largo)*((*entrada).canales+1);
	float* p=(*entrada).muestras;
	float* s=(*salida).muestras;
	float T[1024];// Inicializo una tabla LUT
	int x;
	float c=a*MAX_VAL;		//Obtengo a, a partir del rango como fraccion del valor pico (u)

	for(i=0;i<1024;i++) {
		x=-MAX_VAL-1+i*64;
	
		T[i]=c*(expf(x/c)-expf(-x/c))/(expf(x/c)+expf(-x/c));
	}
	for(i=0;i<largomuestras;i++){

		
		*s=(bloque_lut(*p,T));// Utilizo el bloque LUT con la tabla inicializada
		s++;
		p++;
	}
	hard_clip(salida);
}






void filtro_norm(const audio_t* entrada, float factor, audio_t* salida){
	int i=0;
	int largomuestras=((*entrada).largo)*((*entrada).canales+1);
	float max=0;
	float a,aux,x;
	float T[1024];
	a=MAX_VAL*factor;
	float* p=(*entrada).muestras;
	float* s=(*salida).muestras;

	for(i=0;i<largomuestras;i++) {
		if(abs(*p)>max) {
			max=abs(*p);//Busco el maximo de la entrada
		}
	p++;
	}

	for(i=0;i<1024;i++) { 		//inicializo tabla para hacer soft clipping
		x=-MAX_VAL-1+i*64;
	
		T[i]=a*(expf(x/a)-expf(-x/a))/(expf(x/a)+expf(-x/a));
	}

	p=(*entrada).muestras;

	for(i=0;i<largomuestras;i++){

		aux=(*p)*a/max;
		*s=bloque_lut(aux,T);// Utilizo el bloque LUT con la tabla inicializada
		s++;
		p++;
	}
	hard_clip(salida);
}
	




void filtro_eco(const audio_t* entrada, float retardo,float mezcla, audio_t* salida){
	float r=retardo*((*entrada).frec_muestreo);  //Retraso en muestras
	printf("%f\n",r);
	int i=0;
	int largomuestras=((*entrada).largo)*((*entrada).canales+1);
	float* p=(*entrada).muestras;
	float* s=(*salida).muestras;

	if((*entrada).canales==MONO){		//Si el audio es mono alcanza con un buffer
		buffer_t buffer1;

		inicializar_buffer(&buffer1);

		for(i=0;i<largomuestras;i++) {
		
		*s=bloque_eco(*p,r,mezcla,&buffer1);
		s++;
		p++;
		}
	destruir_buffer(&buffer1);
	}
	else if((*entrada).canales==STEREO){		//Si el audio es stereo preciso 2 buffers

		buffer_t buffer1;
		buffer_t buffer2;

		inicializar_buffer(&buffer1);
		inicializar_buffer(&buffer2);
		
		for(i=0;i<largomuestras;i++) {

			if((i%2)==1){		//Si la muestra es impar, es el canal izquierdo

				*s=bloque_eco(*p,r,mezcla,&buffer1); //Trabajo en el buffer1
			}
			else if ((i%2)==0){ //Si la muestra es para es el canal derecho

				*s=bloque_eco(*p,r,mezcla,&buffer2); //Trabajo en el buffer1
			}
			s++;
			p++;
		}
		destruir_buffer(&buffer1);
		destruir_buffer(&buffer2);
	 }
	hard_clip(salida);


}





void filtro_chorus(const audio_t* entrada, float profundidad_seg, float frecuencia_hz, float mezcla,audio_t* salida){
	int i=0;
	int largomuestras=((*entrada).largo)*((*entrada).canales+1);
	double psi=0;
	double* ppsi=&psi;
	float r=profundidad_seg*((*entrada).frec_muestreo);  //Retraso en muestras
	float* p=(*entrada).muestras;
	float* s=(*salida).muestras;
	float delta; //Retraso por LFO

	if((*entrada).canales==MONO){		//Si el audio es mono alcanza con un buffer
		buffer_t buffer1;

		inicializar_buffer(&buffer1);

		for(i=0;i<largomuestras;i++) {
		
		delta=bloque_lfo(0, frecuencia_hz,r,ppsi, (*entrada).frec_muestreo);
		*s=bloque_eco(*p,r+delta,mezcla,&buffer1);
		s++;
		p++;
		}
	destruir_buffer(&buffer1);
	}
	else if((*entrada).canales==STEREO){		//Si el audio es stereo preciso 2 buffers

		buffer_t buffer1;
		buffer_t buffer2;

		inicializar_buffer(&buffer1);
		inicializar_buffer(&buffer2);
		
		for(i=0;i<largomuestras;i++) {

			delta=bloque_lfo(0, frecuencia_hz,r,ppsi, (*entrada).frec_muestreo);

			if((i%2)==1){		//Si la muestra es impar, es el canal izquierdo

				*s=bloque_eco(*p,r+delta,mezcla,&buffer1); //Trabajo en el buffer1
			}
			else if ((i%2)==0){ //Si la muestra es para es el canal derecho

				*s=bloque_eco(*p,r+delta,mezcla,&buffer2); //Trabajo en el buffer1
			}
			s++;
			p++;
		}
	destruir_buffer(&buffer1);
	destruir_buffer(&buffer2);
	 }
	hard_clip(salida);
}




void filtro_flanger(const audio_t* entrada, float profundidad_seg, float intensidad, float frecuencia_hz, float mezcla,audio_t* salida){


	int i=0;
	int largomuestras=((*entrada).largo)*((*entrada).canales+1);
	double psi=0;
	double* ppsi=&psi;
	float r=profundidad_seg*((*entrada).frec_muestreo);  //Retraso en muestras
	float* p=(*entrada).muestras;
	float* s=(*salida).muestras;
	float delta; //Retraso por LFO

	if((*entrada).canales==MONO){		//Si el audio es mono alcanza con un buffer
		buffer_t buffer1;

		inicializar_buffer(&buffer1);

		for(i=0;i<largomuestras;i++){
		
		delta=bloque_lfo(1, frecuencia_hz,r,ppsi, (*entrada).frec_muestreo);//Onda tri
		*s=bloque_comb(*p,r+delta,intensidad,&buffer1);
		s++;
		p++;
		}
	destruir_buffer(&buffer1);
	}
	else if((*entrada).canales==STEREO){		//Si el audio es stereo preciso 2 buffers

		buffer_t buffer1;
		buffer_t buffer2;

		inicializar_buffer(&buffer1);
		inicializar_buffer(&buffer2);
		
		for(i=0;i<largomuestras;i++){

			delta=bloque_lfo(1, frecuencia_hz,r,ppsi, (*entrada).frec_muestreo);//Onda tri

			if((i%2)==1){		//Si la muestra es impar, es el canal izquierdo

				*s=bloque_comb(*p,r+delta,intensidad,&buffer1); //Trabajo en el buffer1
			}
			else if ((i%2)==0){ //Si la muestra es para es el canal derecho

				*s=bloque_comb(*p,r+delta,intensidad,&buffer2); //Trabajo en el buffer1
			}
			s++;
			p++;
		}
	destruir_buffer(&buffer1);
	destruir_buffer(&buffer2);
	 }
	hard_clip(salida);
}


	
void filtro_reverb(const audio_t* entrada, float profundidad_seg, float intensidad, float mezcla,audio_t* salida){

	int i=0;
	int largomuestras=((*entrada).largo)*((*entrada).canales+1);
	float r=profundidad_seg*((*entrada).frec_muestreo);  //Retraso en muestras
	float* p=(*entrada).muestras;
	float* s=(*salida).muestras;
	float delta;
	if((*entrada).canales==MONO){		//Si el audio es mono alcanza con un buffer
		buffer_t buffer1;

		inicializar_buffer(&buffer1);

		for(i=0;i<largomuestras;i++){

		*s=(*p)*(1-mezcla)+(mezcla/4)*(bloque_comb(*p,r,intensidad,&buffer1)+bloque_comb(*p,r*0.73,intensidad,&buffer1)+bloque_comb(*p,r*0.57,intensidad,&buffer1)+bloque_comb(*p,r*0.37,intensidad,&buffer1));
		


		s++;
		p++;
		}
	destruir_buffer(&buffer1);
	}
	else if((*entrada).canales==STEREO){		//Si el audio es stereo preciso 2 buffers

		buffer_t buffer1;
		buffer_t buffer2;

		inicializar_buffer(&buffer1);
		inicializar_buffer(&buffer2);
		
		for(i=0;i<largomuestras;i++){

			

			if((i%2)==1){		//Si la muestra es impar, es el canal izquierdo

				*s=(*p)*(1-mezcla)+(mezcla/4)*(bloque_comb(*p,r,intensidad,&buffer1)+bloque_comb(*p,r*0.73,intensidad,&buffer1)+bloque_comb(*p,r*0.57,intensidad,&buffer1)+bloque_comb(*p,r*0.37,intensidad,&buffer1));
			}
			else if ((i%2)==0){ //Si la muestra es para es el canal derecho

				*s=(*p)*(1-mezcla)+(mezcla/4)*(bloque_comb(*p,r,intensidad,&buffer2)+bloque_comb(*p,r*0.73,intensidad,&buffer2)+bloque_comb(*p,r*0.57,intensidad,&buffer2)+bloque_comb(*p,r*0.37,intensidad,&buffer2));
			}
			s++;
			p++;
		}
	destruir_buffer(&buffer1);
	destruir_buffer(&buffer2);
	 }
	hard_clip(salida);
}








