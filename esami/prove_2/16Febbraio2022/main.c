#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

typedef int pipe_t[2];

typedef struct{
	int pid;
	long int occ;
}s;

int main(int argc, char **argv){
	int pid;
	int pidFiglio;
	int ritorno;
	int status;
	int fd;
	int nr, nw;
	int N;
	char C;
	pipe_t *piped;
	s *str;

	if(argc < 4){
		printf("errore nel numero di parametri\n");
		exit(1);
	}

	C = argv[argc - 1][0];
	if(strlen(C) != 1){
		printf("errore nell'ultimo parametro\n");
		exit(2);
	}

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

	/*generiamo i figli*/
	for(i = 0; i < N; i++){
		if((pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(5);
		}
		if(pid == 0){
			/*codice figlio*/
			/*chiudiamo i lati della pipe*/
			for(j = 0; j < N; j++){
				/*leggo dall'indice corrente*/
				if(j != i){
					close(piped[j][0]);
				}
				/*scrivo al figlio con indice successsivo*/
				if((i == 0) || (j != i+1)){
					close(piped[j][1]);
				}
			}
			/*apro il file associato*/
			fd = open(argv[i+1], O_RDONLY);
			if(fd < 0){
				printf("errore in apertura file\n");
				exit(-1);
			}

			/*allochiamo memoria per array di strutture*/
			str = (s *)malloc((i+1)*sizeof(s));
			if(str == NULL){
				printf("errore nella malloc della struttura\n");
				exit(-1);
			}

			/*inizializiamo la struttura*/
			str[i].pid = getpid();
			str[i].occ = 0;
			while(read(fd, &ch, 1)){
				if(ch == C){
					(str[i].occ)++;
				}
			}

			if(i !=  0){
				/*leggiamo da figlio precedente*/
				nr = read(piped[i][0], str, (i+1)*sizeof(s));
				if(nr != sizeof(s)){
					printf("errore nella read\n");
					exit(-1);
				}
			}

			/*comunichiamo al figlio successivo l'array*/
			nw = write(piped[i][1], str, (i+1)*sizeof(s));
			exit(i);
		}
	}
	/*codice padre*/
	/*chiudiamo i lati della pipe*/
	for(i = 0; i < N; i++){
		close(piped[i][1]);
		if(i = N-1){
			close(piped[i][0]);
		}
	}

	str = (s *)malloc(N*sizeof(s));
	if(str == NULL){
		printf("errore nella malloc\n");
		exit(7);
	}

	nr = read(piped[i][0], str, N*sizeof(s));
	if(nr != N*sizeof(s)){
		printf("errore nella read\n");
		exit(8);
	}
	nr=nr/sizeof(s);

	for(i = 0; i < N; i++){
		printf("figlio di indice %d e pid %d ha trovato %ld occorrenze nel file %s\n", i, str[i].pid, str[i].occ, argv[i+1]);
	}

	/*il padre aspetta i figli*/

	exit(0);
}


