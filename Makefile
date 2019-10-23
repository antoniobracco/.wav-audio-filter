obligatorio: main.o filtros.o bloques.o audio.o buffer.o filtros.h bloques.h audio.h buffer.h
	gcc -o Audio_filter main.o filtros.o bloques.o audio.o buffer.o -lm

main.o: main.c
	gcc -c main.c

filtros.o: filtros.c filtros.h
	gcc -c filtros.c

bloques.o: bloques.c bloques.h
	gcc -c bloques.c

buffer.o: buffer.c buffer.h
	gcc -c buffer.c

audio.o: audio.c audio.h
	gcc -c audio.c

clean:
	rm -r *.o
