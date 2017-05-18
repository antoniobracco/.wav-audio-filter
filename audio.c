#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "audio.h"


void inicializar_audio(audio_t* audio){

	unsigned int espacio=((*audio).largo)*((*audio).canales+1);

	(*audio).muestras=(float *)malloc(espacio*sizeof(float));
	

}



void destruir_audio(audio_t *audio){

free((*audio).muestras);
(*audio).muestras=NULL;
}


int bytesaentero(char bytes[4]) {

	int* valor=(int*)bytes;

	return(*valor);
}

short bytesashort(char bytes[2]) {

	/*short valor;

	valor = bytes[1] << 8 | bytes[0];

	return(valor);*/	
	short* valor=(short*)bytes;

	return(*valor);
}



wav_resultado_t leer_wav(const char* ruta, audio_t *audio){

	FILE *fp=fopen(ruta,"r"); //Abro el archivo en modo lectura

	
	int i=0;
	char c,d;
	char str[44];
	short canales;
	unsigned int nbc;



	while (i<44) {

	str[i]=getc(fp);
	i++;
	}



	char can[]={str[22],str[23]};
	canales=bytesashort(can);				//Consigo la info por partes
	if (canales==2){
		(*audio).canales=STEREO;
	}
	else {
		(*audio).canales=MONO;			// Primero la cantidad de canales
		}
	
	printf("%s %d \n","canales",canales);

	char frec[]={str[24],str[25],str[26],str[27]};	// Luego la frecuencia de muestreo
	(*audio).frec_muestreo=bytesaentero(frec);
	
	printf("%s %d \n","frecuencia",(*audio).frec_muestreo);

	char totalbytes[]={str[40],str[41],str[42],str[43]};	//Entre los bytes por muestra (2) y el total
	nbc=bytesaentero(totalbytes);					// de bytes, obtengo el largo (N)
	(*audio).largo=nbc/(2*((*audio).canales+1));					//Con el

	printf("%s %d \n","nbc",nbc);
	inicializar_audio(audio);
	float *p=(*audio).muestras;				// Inicializo el audio (p queda señalando
								// a muestras)
	short muestrashort;
	char muestraleida[2];
	//char muestraleida[2];
	i=0;
	while(i<nbc/2)	{	//Para cada pack de 2 bytes de datos
		muestraleida[0]=getc(fp);
		muestraleida[1]=getc(fp);
		muestrashort=bytesashort(muestraleida);
		*p=(float)muestrashort;
		p++;
		i++;
		}

fclose(fp);
return(0);
}		



wav_resultado_t escribir_wav(const char* ruta,audio_t *audio){



FILE *fp=fopen(ruta,"w");
unsigned int bcn= ((*audio).largo)*((*audio).canales+1)*2;
unsigned int largototal=bcn+36;
int i;


char* c=(char *)(&largototal);
fputc('R',fp);
fputc('I',fp);
fputc('F',fp);
fputc('F',fp);

fprintf(fp,"%c",*c);
c++;
fprintf(fp,"%c",*c);
c++;
fprintf(fp,"%c",*c);
c++;
fprintf(fp,"%c",*c);


fputc('W',fp);
fputc('A',fp);
fputc('V',fp);
fputc('E',fp);
fputc('f',fp);
fputc('m',fp);
fputc('t',fp);
fputc(' ',fp);

int largoencabezado=16;
c=(char *)(&largoencabezado);

fprintf(fp,"%c",*c);
c++;
fprintf(fp,"%c",*c);
c++;
fprintf(fp,"%c",*c);
c++;
fprintf(fp,"%c",*c);

short uno=1;
c=(char *)(&uno);
fprintf(fp,"%c",*c);
c++;
fprintf(fp,"%c",*c);


short canales=(short)((*audio).canales+1);
c=(char *)(&canales);

fprintf(fp,"%c",*c);
c++;
fprintf(fp,"%c",*c);


int F=(*audio).frec_muestreo;
c=(char *)(&F);
fprintf(fp,"%c",*c);
c++;
fprintf(fp,"%c",*c);
c++;
fprintf(fp,"%c",*c);
c++;
fprintf(fp,"%c",*c);

int FBC=F*canales*2;
c=(char *)(&FBC);
fprintf(fp,"%c",*c);
c++;
fprintf(fp,"%c",*c);
c++;
fprintf(fp,"%c",*c);
c++;
fprintf(fp,"%c",*c);

short BC=2*canales;
c=(char *)(&BC);
fprintf(fp,"%c",*c);
c++;
fprintf(fp,"%c",*c);

short bits=16;
c=(char *)(&bits);
fprintf(fp,"%c",*c);
c++;
fprintf(fp,"%c",*c);

fputc('d',fp);
fputc('a',fp);
fputc('t',fp);
fputc('a',fp);

int BECENE=(int)bcn;
c=(char *)(&BECENE);
fprintf(fp,"%c",*c);
c++;
fprintf(fp,"%c",*c);
c++;
fprintf(fp,"%c",*c);
c++;
fprintf(fp,"%c",*c);

float* p=(*audio).muestras;
short muestrap;

for(i=0;i<(bcn/2);i++) {

	muestrap=(short)(*p);
	c=(char *)(&muestrap);
	fprintf(fp,"%c",*c);
	c++;
	fprintf(fp,"%c",*c);
	p++;
	}

p=NULL;

fclose(fp);
return(0);
}























