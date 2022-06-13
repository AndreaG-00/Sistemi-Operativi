#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

typedef int pipe_t[2];

typedef struct{
	int c1;
	int c2;
}s;

int main(int argc, char **argv){
	int pid;
	int pidFiglio;
	int ritorno;
	int status;
	int fd;
	int nr, nw;
	int N;
	int i, j;
	char ch;
	//pipe_t *pipe_pf;  per inviare ok dopo aver ricevuto tutti i dati da processi pari
	pipe_t *pipe_fp;
	s struttura;
	int pos;

	if(argc < 3){
		printf("errore nel numero di parametri\n");
		exit(1);
	}
	
	N = argc - 1;

	if((N % 2) != 0){
		printf("errore numero processi non pari\n");
		exit(2);
	}

	/*allochiamo memoria per le pipe*/
	//pipe_pf = (pipe_t *)malloc(N*sizeof(pipe_t));
	pipe_fp = (pipe_t *)malloc(N*sizeof(pipe_t));
	if((pipe_fp == NULL)){ //|| (pipe_pf == NULL)){
		printf("errore nella malloc pipe\n");
		exit(3);
	}

	/*creiamo le pipe*/
	for(i = 0; i < N; i++){
		if(pipe(pipe_fp[i]) < 0){
			printf("errore nella creazione della pipe f-p\n");
			exit(4);
		}
		/*
		if(pipe(pipe_pf[i]) < 0){
			printf("errore nella creazione della pipe p-f\n");
			exit(5);
		}
		*/
	}

	/*creazione figli*/
	for(i = 0; i < N; i++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(6);
		}
		if(pid == 0){
			/*codice figlio*/
			/*chiudiamo i lati della pipe*/
			for(j = 0; j < N; j++){
				close(pipe_fp[j][0]);
				//close(pipe_pf[j][1]);
				if(j != i){
					close(pipe_fp[j][1]);
					//close(pipe_pf[j][0]);
				}
			}
			/*apriamo il file associato*/
			fd = open(argv[i+1], O_RDONLY);
			if(fd < 0){
				printf("errore in apertura file\n");
				exit(-1);
			}

			if((i % 2) == 0){
				/*processo pari*/
				struttura.c1 = 0;
				struttura.c2 = 0;
				pos=0;
				while(read(fd, &ch, 1)){
					if((pos % 2) == 0){
						/*solo caratteri di posizione pari*/
						if((ch % 2) == 0){
							/*codice ascii pari*/
							struttura.c1++;
						}else{
							/*codice ascii dispari*/
							struttura.c2++;
						}
					}
					pos++;
				}
				/*ora invio al padre i conteggi fatti*/
				nw = write(pipe_fp[i][1], &struttura, sizeof(s));
				if(nw != sizeof(s)){
					printf("errore nella write\n");
					exit(-1);
				}

				/*ora controlliamo che c1 > c2*/
				if(struttura.c1 > struttura.c2){
					exit(0);
				}else{
					exit(1);
				}
			}else{
				/*processo dispari*/
				struttura.c1=0;
				struttura.c2=0;
				pos=0;
				while(read(fd, &ch, 1)){
					if((pos % 2) != 0){
						/*solo caratteri dispari*/
						if((ch % 2) == 0){
							/*codice ascii pari*/
							struttura.c1++;
						}else{
							/*codice ascii dispari*/
							struttura.c2++;
						}
					}
					pos++;
				}
				nw = write(pipe_fp[i][1], &struttura, sizeof(s));
				if(nw != sizeof(s)){
					printf("errore nella write\n");
					exit(-1);
				}

				if(struttura.c1 > struttura.c2){
					exit(0);
				}else{
					exit(1);
				}
			}
		}
	}
	/*codice padre*/
	/*chiudiamo i lati della pipe*/
	for(i = 0; i < N; i++){
		close(pipe_fp[i][1]);
	}

	/*leggiamo le info da pipe*/
	/*processi pari*/
	for(i = 0; i < N; i=i+2){
		nr = read(pipe_fp[i][0], &struttura, sizeof(s));
		if(nr != sizeof(s)){
			printf("errore nella read\n");
			exit(8);
		}
		printf("figlio di indice %d e pid %d ha inviato al padre le due strutture: c1 -> %d, c2 -> %d\n", i, getpid(), struttura.c1, struttura.c2);
	}

	/*stampiamo le info dei processi dispari*/
	for(i = 1; i < N; i=i+2){
		nr = read(pipe_fp[i][0], &struttura, sizeof(s));
		if(nr != sizeof(s)){
			printf("errore nella read\n");
			exit(10);
		}
		printf("figlio di indice %d e pid %d ha inviato al padre le due strutture: c1 -> %d, c2 -> %d\n", i, getpid(), struttura.c1, struttura.c2);
	}

	/*il padre aspetta i figli*/
	for(i = 0; i < N; i++){
		if((pidFiglio = wait(&status)) < 0){
			printf("errore nella wait\n");
			exit(11);
		}
		if((status & 0xFF) != 0){
			printf("figlio terminato in modo anomalo\n");
		}else{
			ritorno = ((status >> 8) & 0xFF);
			printf("figlio con pid %d ha ritornato %d\n", pidFiglio, ritorno);
		}
	}
	exit(0);
}



