#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bloques.h"
#include "filtros.h"
#include <string.h>
#include "audio.h"
#include "buffer.h"




int main(int argc,char** argv){


audio_t entrada;
audio_t salida;

entrada.muestras=NULL;
salida.muestras=NULL;
int i=0;

if(argc<4) {
	return(30);
}

float param[4]={-1,-1,-1,-1}; // Cuando los parametros no sean aclarados quedaran en -1 para luego ser
				// definidos con el valor estandar

for (i=0;i<argc-4;i++) {

	param[i]=atof(argv[i+4]);
}


int resultado_lectura=leer_wav(argv[1],&entrada);


if (resultado_lectura==0) {
	
salida.canales=entrada.canales;	//La salida sera del mismo tipo que la entrada con muestras
salida.frec_muestreo=entrada.frec_muestreo;// alteradas pero mismo largo, frecuencia y cantidad de canales
salida.largo=entrada.largo;


inicializar_audio(&salida); //Reservo el espacio para la salida



	if(!strcmp(argv[3],"amp")) {

		if(param[0]==-1) {
			param[0]=1.0;
		}

		printf("Parametro amp %f\n",param[0]);
		filtro_amp(&entrada,param[0],&salida);
	}	




	else if(!strcmp(argv[3],"norm")) {


		if(param[0]==-1) {
		param[0]=1.0;
		}
	
		filtro_norm(&entrada,param[0],&salida);
	
	}

	else if(!strcmp(argv[3],"over")) {

		if(param[0]==-1) {
		param[0]=3.0;
		}
	
		filtro_over(&entrada,param[0],&salida);

	}


	else if(!strcmp(argv[3],"clip")) {

		if(param[0]==-1) {
		param[0]=1.0;
		}
	
		filtro_clip(&entrada,param[0],&salida);
	
	}


	else if(!strcmp(argv[3],"eco")) {


		if(param[0]==-1) {
		param[0]=0.1;
		}

		if(param[1]==-1) {
		param[1]=0.5;
		}
	
		filtro_eco(&entrada,param[0],param[1],&salida);

	}

	

	

	else if(!strcmp(argv[3],"chorus")) {

		if(param[0]==-1) {
		param[0]=0.005;
		}

		if(param[1]==-1) {
		param[1]=0.5;
		}

		if(param[2]==-1) {
		param[2]=0.5;
		}
	
		filtro_chorus(&entrada,param[0],param[1],param[2],&salida);

	}

	else if(!strcmp(argv[3],"flanger")) {

		if(param[0]==-1) {
		param[0]=0.001;
		}

		if(param[1]==-1) {
		param[1]=0.8;
		}

		if(param[2]==-1) {
		param[2]=0.2;
		}

		if(param[3]==-1) {
		param[3]=1.0;
		}
	
		filtro_flanger(&entrada,param[0],param[1],param[2],param[3],&salida);
	}
	
	else if(!strcmp(argv[3],"reverb")) {


		if(param[0]==-1) {
		param[0]=0.15;
		}

		if(param[1]==-1) {
		param[1]=0.8;
		}

		if(param[2]==-1) {
		param[2]=0.9;
		}
	
		filtro_reverb(&entrada,param[0],param[1],param[2],&salida);
	}	

	//printf("5/n");
	int resultado_escritura=escribir_wav(argv[2],&salida);
	printf("resultado %d \n",resultado_escritura);
	destruir_audio(&entrada);
	destruir_audio(&salida);
	//printf("7/n");

	return(0);

}

else {
destruir_audio(&entrada);
return(-1);
}
}

