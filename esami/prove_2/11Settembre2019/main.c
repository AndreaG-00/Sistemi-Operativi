#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <ctype.h>

typedef int pipe_t[2];

int main(int argc, char **argv){
	int pid;
	int pidFiglio;
	int ritorno;
	int status;
	int fd;
	int N;
	int i, j;
	int nr, nw;
	pipe_t *pipe_pd;
	//pipe_t *pipe_dp;
	char *Fcreato;
	int fcreato;
	int nro;
	char ch, ch2;

	if(argc < 3){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	if((argc - 1) % 2 != 0){
		printf("errore parametri non di numero pari\n");
		exit(2);
	}

	N = argc - 1;

	/*allochiamo le pipe*/
	pipe_pd = (pipe_t *)malloc((N/2)*sizeof(pipe_t));
	//pipe_dp = (pipe_t *)malloc(N*sizeof(pipe_t));
	if(pipe_pd == NULL){
		printf("errore nella malloc della pipe\n");
		exit(3);
	}

	/*creiamo le pipe*/
	for(i = 0; i < N/2; i++){
		if(pipe(pipe_pd[i]) < 0){
			printf("errore nella creazione della pipe pari-dispari\n");
			exit(4);
		}
		/*
		if(pipe(pipe_dp[i]) < 0){
			printf("errore nella creazione della pipe dispari-pari\n");
			exit(5);
		}
		*/	
	}

	/*generiamo i figli*/
	for(i = 0; i < N; i++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(6);
		}
		if(pid == 0){
			/*codice figlio*/
			nro=0;
			if((i % 2) == 0){
				/*processo pari*/
				/*chiudiamo i lati della pipe*/
				for(j = 0; j < N/2; j++){
					close(pipe_pd[j][0]);
					if(j != i/2){
						close(pipe_pd[j][1]);
					}
				}
				/*apriamo il file associato*/
				fd = open(argv[i+1], O_RDONLY);
				if(fd < 0){
					printf("errore in apertura file\n");
					exit(-1);
				}

				while(read(fd, &ch, 1)){
					nw = write(pipe_pd[i/2][1], &ch, 1);
					if(nw != 1){
						printf("errore nella scrittura su pipe\n");
						exit(-1);
					}
					nro++;
				}
			}else{
				/*processo dispari*/
				/*chiudiamo i lati della pipe*/
				for(j = 0; j < N/2; j++){
					close(pipe_pd[j][1]);
					if(j != i/2){
						close(pipe_pd[j][0]);
					}
				}
				/*apriamo il file associato*/
				fd = open(argv[i+1], O_RDONLY);
				if(fd < 0){
					printf("errore in apertura file\n");
					exit(-1);
				}

				/*creiamo il file Fcreato*/
				Fcreato = (char *)malloc(strlen(argv[i+1]) + 10);
				if(Fcreato == NULL){
					printf("errore nella malloc di Fcreato\n");
					exit(-1);
				}

				sprintf(Fcreato, "%s.MAGGIORE", argv[i+1]);
				fcreato = creat(Fcreato, 0644);
				if(fcreato < 0){
					printf("errore nella creazione del file\n");
					exit(-1);
				}
				
				while(read(fd, &ch2, 1)){
					nr = read(pipe_pd[i/2][0], &ch, 1);
					if(nr != 1){
						printf("errore nella read\n");
						exit(-1);
					}
					if(ch2 > ch){
						/*scriviamo ch2 su Fcreato*/
						write(fcreato, &ch2, sizeof(char));
					}else{
						write(fcreato, &ch, sizeof(char));
					}
					nro++;
				}
			}	
			exit(nro);		
		}
	}
	/*codice padre*/
	/*chiudiamo i lati della pipe*/
	for(i = 0; i < N; i++){
		close(pipe_pd[i][1]);
		close(pipe_pd[i][0]);
	}

	/*il padre aspetta i figli*/
	for(i = 0; i < N; i++){
		if((pidFiglio = wait(&status)) < 0){
			printf("errore nella wait\n");
			exit(8);
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
