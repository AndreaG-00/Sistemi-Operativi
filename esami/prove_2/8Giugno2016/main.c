#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h>

typedef int pipe_t[2];

int mia_random(int n){
	int casuale;
	casuale = rand()%n;
	return casuale;
}

int main(int argc, char **argv){
	int pid; 
	int pidFiglio;
	int ritorno;
	int status;
	int fd;
	int Fcreato;
	int N;
	int i, j;
	int nr, nw;
	pipe_t *pipe_pf;
	pipe_t *pipe_fp;
	int H;
	char ch;

	if(argc < 6){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	H = atoi(argv[argc - 1]);
	if((H <= 0) || (H > 255)){
		printf("errore nell'ultimo parametro\n");
		exit(2);
	}

	N = argc - 2;

	srand(time(NULL));
	
	/*creiamo il file Fcreato*/
	Fcreato = creat("/tmp/creato", 0644);
	if(Fcreato < 0){
		printf("errore nella creazione file\n");
		exit(3);
	}

	/*allochiamo memoria per le pipe*/
	pipe_pf = (pipe_t *)malloc(N*sizeof(pipe_t));
	pipe_fp = (pipe_t *)malloc(N*sizeof(pipe_t));
	if((pipe_pf == NULL) || (pipe_fp == NULL)){
		printf("errore nella malloc\n");
		exit(4);
	}

	/*creiamo le pipe*/
	for(i = 0; i < N; i++){
		if(pipe(pipe_pf[i]) < 0){
			printf("errore nella creazione della pipe p-f\n");
			exit(5);
		}
		if(pipe(pipe_fp[i]) < 0){
			printf("errore nella creazione della pipe f-p\n");
			exit(6);
		}
	}

	/*creiamo i figli*/
	for(i = 0; i < N; i++){
		if((pid = fork()) < 0){
			printf("errore fork\n");
			exit(7);
		}
		if(pid == 0){
			/*codice figlio*/
			/*chiudiamo i lati della pipe*/
			for(j = 0; j < N; j++){
				close(pipe_pf[j][1]);
				close(pipe_fp[j][0]);
				if(j != i){
					close(pipe_pf[j][0]);
					close(pipe_fp[j][1]);
				}
			}
			/*apriamo il file associato*/
			fd = open(argv[i+1], O_RDONLY);
			if(fd < 0){
				printf("errore nella open\n");
				exit(-1);
			}

			j = 0;
			while(read(fd, &(linea[j]), 1)){
				if(linea[j] == '\n'){
					/*comunichiamo al padre la lunghezza della linea corrente*/
					j++;
					nw = write(pipe_fp[i][1], &j, sizeof(j));
					if(nw != sizeof(lenght)){
						printf("errore nella write\n");
						exit(-1);
					}
					nr = read(pipe_pf[i][0], &r, sizeof(r));
					if(nr != sizeof(r)){
						printf("errore nella read\n");
						exit(-1);
					}
					if(r < j){
						write(Fcreato, &(linea[r]), 1);
						ritorno++;
					}
					j=0;
				}else{
					j++;
				}
			}
			exit(ritorno);
		}
	}
	/*codice padre*/
	/*chiudiamo i lati della pipe*/
	for(i = 0; i < N; i++){
		close(pipe_pf[i][0]);
		close(pipe_fp[i][1]);
	}

	for(j = 1; j <= H; j++){
		r = mia_random(N);
		for(i = 0; i < N; i++){
			read(pipe_fp[i][0], &valore, sizeof(valore));
			if(i == r){
				giusto=valore;
			}
		}
		r=mia_random(giusto);
		for(i = 0; i < N; i++){
			write(pipe_pf[i][1], &r, sizeof(r));
		}
	}
	/*il padre aspetta i figli*/

	exit(0);
}
