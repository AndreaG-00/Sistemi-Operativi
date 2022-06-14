#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

typedef int pipe_t[2];

typedef struct{
	int pid;
	int lenght;
}s;

int main(int argc, char **argv){
	int pid;
	int pidFiglio;
	int ritorno;
	int status;
	int fd;
	int N;
	int i, j;
	pipe_t *piped;
	int Y;
	char ch;
	s *str;

	if(argc < 4){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	Y = atoi(argv[i-1]);

	if(Y <= 0){
		printf("errore nell'ultimo parametro, non positivo\n");
		exit(2);
	}

	N = argc - 2;

	/*allochiamo memoria per la pipe*/
	piped = (pipe_t *)malloc(N*sizeof(pipe_t));
	if(piped == NULL){
		printf("errore nella malloc\n");
		exit(3);
	}

	/*creiamo la pipe*/
	for(i = 0; i < N; i++){
		if(pipe(piped[i]) < 0){
			printf("errore nella creazione della pipe\n");
			exit(4);
		}
	}

	/*creazione figli*/
	for(i = 0; i < N; i++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(5);
		}
		if(pid == 0){
			/*codice figlio*/
			/*chiudiamo i lati della pipe*/
			for(j = 0; j < N; j++){
				if(j!=i){
					close(piped[j][0]);
				}
				if(){}
			}
			str = (s *)malloc(
		}
	}
	/*codice padre*/
	exit(0);
}

	
