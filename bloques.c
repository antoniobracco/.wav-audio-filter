//Bloques para el obligatorio

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "audio.h"
#include "buffer.h"


//BLOQUE LUT

float bloque_lut(float x, float T[1024]) {

float d,y;      // Se utilizaran como booleanas para ver si quedamos antes del primer valor
int i=0;	// 


while((x>=-MAX_VAL-1+i*64)&&(x<=MAX_VAL+1-64)){ //si x esta en el menor y el mayor valor de la tabla
						//busco la ubicacion, dejando i en que le siga
	i++;
}


if((x==-MAX_VAL-1+(i-1)*64)) { //si x era uno de los valores de la tabla
	return(T[i-1]);

}
else if(x>MAX_VAL+1-64) { //si x era mayor a cualquier elemento
	return(T[1023]);

}
else if(i==0) { //si x era menor a cualquier elemento
	return(T[0]);
}
else {

	d=x+MAX_VAL+1-(i-1)*64;

	y=T[i-1]+(T[i]-T[i-1])*(d/64);

	return(y);
	}
}






//BLOQUE LFO

//Primero defino 2 funciones auxiliares, t y s
float s(double fase, float a, float frec) {
const float PI=3.14159;
	return(a*sin(2*PI*frec*fase));
	}


float t(double fase, float a, float frec) {

	if(fase<1/(2*frec)) {
		return(-a+4*a*fase*frec);
	}
	else{
		return(a-4*a*frec*(fase-1/(2*frec)));
	}
}



float bloque_lfo(int tipo, float f0, float a, double* ppsi, unsigned frec_muestreo){

float res;

	if(tipo==0) {
		res=s(*ppsi,a,f0);
		}
	else{
		res=t(*ppsi,a,f0);
	}

	*ppsi=fmod((*ppsi+1.0/(double)frec_muestreo),1/f0);

	return(res);
	}




	

//BLOQUE ECO

//Guarda la muestra en el buffer, luego devuelve el dato retrasado en ret

float bloque_eco(float muestra, float ret, float mezcla, buffer_t *buffer){
	
	float f;//aqui almacenare la muestra retrasada

	
	

	escribir_buffer(muestra,buffer); //Primero escribo la muestra

	
	f=leer_buffer(ret,buffer);
	
	
	
	
	
	return(mezcla*f+(1-mezcla)*muestra);


}



float bloque_comb (float muestra, float ret, float intensidad, buffer_t *buffer){

	float f,g;//aqui almacenare la muestra retrasada y el valor a escribir
	
	f=leer_buffer(ret,buffer);// PRIMERO LEO LA MUESTRA

	g=intensidad*f+muestra;

	escribir_buffer(g,buffer); //Luego escribo una combinacion de la muestra y lo leido
	
	return(g);
}


	












