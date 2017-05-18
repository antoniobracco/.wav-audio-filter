#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "audio.h"
#include "buffer.h"


void inicializar_buffer(buffer_t *buffer){
	int i=0;
	(*buffer).almacenamiento=(float *)malloc(BUFFER_SIZE*(sizeof(float)));

	float* p=(*buffer).almacenamiento;

	for(i=0;i<BUFFER_SIZE;i++){

		*p=0;
		p++;
	}
	
	(*buffer).posicion=0;
}



float leer_buffer(float retraso, buffer_t *buffer){
	float* p;
	p=(*buffer).almacenamiento;

	float j=floorf(retraso);
	int resta=(int)((*buffer).posicion-j);

	if(j==retraso){		//Si el retraso es entero, no hay que interpolar
		
		
		if(resta>=0){		//Obtengo el valor para el retraso j
			p+=(resta%BUFFER_SIZE);
			
			return(*p);
		}
		else if(resta<0){
			p+=((resta%BUFFER_SIZE)+BUFFER_SIZE)%BUFFER_SIZE;
			return(*p);
		}
	}
	
	else{	
		

		
		float a,b,d; //variables a usar para la interpolacion	

		if(resta-1>=0){		//Obtengo el valor para el retraso j+1
			p+=((resta-1)%BUFFER_SIZE);
			a=*p;
		}
		else if(resta-1<0){
			p+=(((resta-1)%BUFFER_SIZE)+BUFFER_SIZE)%BUFFER_SIZE;
			a=*p;
		}

		p=(*buffer).almacenamiento;    //Reinicio el puntero hacia el inicio del buffer

		if(resta>=0){		//Obtengo el valor para el retraso j
			p+=(resta%BUFFER_SIZE);
			b=*p;
		}
		else if(resta<0){
			p+=((resta%BUFFER_SIZE)+BUFFER_SIZE)%BUFFER_SIZE;
			b=*p;
		}


		d=1+j-retraso;

		
		return(a+(b-a)*d);
	}
}

		






void escribir_buffer(float muestra, buffer_t *buffer){

	(*buffer).posicion=((*buffer).posicion+1)%BUFFER_SIZE;
	float* p=(*buffer).almacenamiento +(*buffer).posicion;
	*p=muestra;
	
	
	
	
}
	





void destruir_buffer(buffer_t *buffer){

	free((*buffer).almacenamiento);
	(*buffer).almacenamiento=NULL;
}




















