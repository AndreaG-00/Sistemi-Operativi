#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

typedef int pipe_t[2];

int main(int argc, char **argv){
	int pid;
	int pidFiglio;
	int ritorno = 0;
	int status;
	int fd;
	int B;
	int q, j;
	char *buff;
	pipe_t *piped;
	int nr, nw;
	int B, L;
	int F;

	if(argc != 4){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	B = atoi(argv[2]);
	L = atoi(argv[3]);
	if((B <= 0) || (L <= 0)){
		printf("errore numeri non positivi\n");
		exit(3);
	}

	F = open(argv[1], O_RDONLY);
	if(F < 0){
		printf("errore in apertura file\n");
		exit(3);
	}

	/*allochiamo memoria per la pipe*/
	piped = (pipe_t *)malloc(B*sizeof(pipe_t));
	if(piped == NULL){
		printf("errore nella malloc\n"):
			exit(4);
	}

	/*creiamo la pipe*/
	for(q = 0; q < B; q++){
		if(pipe(piped[q]) < 0){
			printf("errore nella creazione della pipe\n");
			exit(5);
		}
	}

	/*creazione figli*/
	for(q = 0; q < B; q++){
		if((pid = fork()) < 0){
			printf("errore fork\n");
			exit(6);
		}
		if (pid == 0){
			/*codice figlio*/
			/*chiuiamo i lati della pipe*/
			for(j = 0; j < B; j++){
				close(piped[j][0]);
				if(j != q){
					close(piped[j][1]);
				}
			}

			F = open(argv[1], O_RDONLY);
			if(F < 0){
				printf("errore nella open\n");
				exit(-1);
			}

			buff = (char *)malloc(L/B*sizeof(char));
			if(buff == NULL){
				printf("errore nella malloc\n");
				exit(-1);
			}
			lseek(F, (long)q*L/B, SEEK_SET);
			read(fd, buff, L/B);
			write(piped[q][1], &buff[L/B-1], 1);
			exit(L/B);
		}
	}
	/*codice padre*/
	for(q = 0; q < B; q++){
		close(piped[q][1]);
	}

	for(q = 0; q < B; q++){
		read(piped[q][0], &ch, 1);
		printf("figlio con indice %d e pid %d ha letto dal file %s ultimo carattere del blocco %d-esimo: %c\n", q, getpid(), argv[1], q, ch);
	        write(F, &ch, 1);
	}
	/*il padre aspetta i figli*/	
	exit(0);
}




