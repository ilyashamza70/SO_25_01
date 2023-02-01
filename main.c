/*
******  Prova Del 25 Gennaio 2023 QUasi terminata correzione da inviare OBBLIGATORIA
******  FAI DEBUGGING
*/

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <limits.h>


/* VARIABILI GLOBALI */
int *finito; 	/* array dinamico per indicare i figli che sono terminati */
                /* Se vale 1 allora il figlio ha terminato altrimenti NO */
int M; 		    /* numero di processi figli */

//Funzione usata per verificare in contenuto dell'array finito e controllare che siano terminati tutti i figli
//Se tutti terminati return=1 else =0
int finitof()
{
   int i;
    for (i=0; i < M; i++){
	    if (!finito[i]){
		    return 0;       //Qua ho trovato almeno un figlio che non ha terminato cioe finito[i]=0;
        }
        return 1;               //TErminati tutti i figli
    }
}


typedef int pipe_t[2]; 		/* tipo di dato per contenere i file descriptors di una pipe */

int main(int argc,char **argv){
    /*-----------------------------VARIABILI------------------------------------*/
    int pid;				    /* variabile per fork */
    pipe_t *pipe_pf;			/* array di pipe per l'indicazione padre-figli */
    int fd;				        /* variabile per open di tutti i file*/
    int FDW;                    /* variabile per file desriptod del file aperto in scrittura*/
    char *Chs;				    /* array per leggere 3 char dal file alla volta */
	int ritorno;		        /* numero di occorrenze trovate che uso per calcolare status ricevuto */
    int status, pidFiglio;	    /* per wait */
    int i, j, m;				/* indici per cicli */
    int nr, nw; 				/* per controlli read e write su/da pipe */
    int cont;                   /* variabile per contare caratteri*/ 
    /*----------------------------------------------------------------------------*/

    //Prima di tutto eseguo il controllo che M sia maggiore, uguale di 2
    if(argc-1 < 2){
        printf("ERROR: numero di argomenti passati = %d sono insufficente\n",argc - 1);
        exit(1);
    }

    //salvo prima il numero di parametri
    M = argc-2; //Numero di parametri passati;
    //Devo poi aprire il File FM+1 cioè l' ultimo in LETTURA nel Padre
    if((FDW = open(argv[M+1],O_WRONLY)) < 0){
        printf("ERROR nell' apertura del file M+1 di nome %s\n",argv[M+1]);
        exit(2);
    }
    
    //alloco memoria per l'array contenete i miei 3 caratteri letti
    Chs = malloc(3 * sizeof(char));
    if(Chs == NULL){
        printf("ERRORE in allocazione memoria del buffer Chs\n");
        exit(3);
    }
    
    //alloco memoria dinamica per array finito e inizializzo a 0
	finito = (int *) calloc(M,sizeof(int));
	if(finito == NULL)
	{
		printf("Errore nella allocazione della memoria per array finito\n");
		exit(3);
	}
    
    //Alloco la memoria per le pipe
    pipe_pf = malloc(M * sizeof(pipe_t));
    if(pipe_pf == NULL){
        printf("ERRORE in allocazione memoria delle pipe\n");
        exit(3);
    }
    //Ora il padre deve creare esattamente M pipe tra padre e figlio
    for(i=0;i < M;++i){
        if(pipe(pipe_pf[i])!=0){
            printf("Errore creazione delle pipe padre-figli\n");
			exit(5);
        }
    }
    //printf("++++++DEBUG-Sono il processo padre con pid %d e sto per generare %d figli\n", getpid(), M);
    //Uso la variabile m per creare i figli
    for(int m=0;m < M; ++m ){
        if((pid=fork()) < 0){
            printf("Errore nella fork con indice %d\n", m);
      		exit(7);
        }
        //printf("..........DEBUG-CREATO Figlio di indice %d e pid %d associato al file %s\n",m,getpid(), argv[m+1]);
        if(pid == 0){
            ///////////////////////////////////////////////////////
            //-----------------SONO NEL FIGLIO-----------------//
            //printf("DEBUG-Figlio di indice %d e pid %d associato al file %s\n",m,getpid(), argv[m+1]);

            //Ora chiudo le pipe che non servono quindi quelle in lettura
            for (j=0;j<M;j++)
      		{
    			close(pipe_pf[m][0]);
      		}

            //Ora apro il file associato argv[m]
            if((fd = open(argv[m+1],O_RDONLY)) < 0){
                printf("ERROR nell' apertura del file %s\n",argv[m+1]);
                exit(9);
            }
            //Ora da tale file devo leggere 3 caratteri alla volta e inviarli
            
            while((nr = read(fd,Chs,3)) > 0){
                //Invia il Chs a padre
                //printf("Letto valori '%s' dal file %s \n",Chs,argv[m+1]);
                nw=write(pipe_pf[m][1],Chs,3);
                if(nw < 0){
                    printf("ERRORE nella scrittura su pipe del figlio con pid= %d\n",getpid());
                    exit(10);

                }
                cont=cont+nr;
            }
            // printf("++++----CIAOO fine file ultimo invio ha mandato: %s\n",Chs);
            if(nr < 0){
                printf("ERRORE nella lettura dal file  %s\n",argv[m+1]);
                exit(10);
            }
            //Ho terminato la lettura  e l' invio al padre dei 3 caratteri alla volta
            //Ora invi il num di caratteri che ho contato come status
            //Ho terminato con successo lettura.
            exit(cont);
        }
    }
    //printf("------DEBUG-Padre\n");
    //Chioudo le pipe in scrittura
    //Poi leggo dalle pipe,
    for(j=0;j<M;++j){
        close(pipe_pf[j][1]);
    }
    //Lettura pipe finche non terminano tutti
    while(!finitof()){

        for(i=0;i<M;++i){
            nr = read(pipe_pf[i][0],Chs,3);
	        if (nr < 0)
	        {
	    	    printf("Errore in lettura da pipe[N-1] per il padre\n");
            	exit(4);
	        }
            if(nr < 3){
                //Terminato figlio allora setto il valore del figlio a 1nell ' array dinamico
                finito[i]=1;
                break;
            }
            //printf("+-+-+-+-+-+-+-+-ricevuti i caratteri %s su pipe %d, ora scrivili su file \n",Chs,i);
            //Altrimenti scrivo i 3 char lett
            if((nw = write(FDW,Chs,3))< 0){
                printf("Errore in scritturasu file  da pipe[%d] per il padre\n",i);
   
            }
            

        }
    }
    //Ora attendo i figli e il relativo numero di caratteri letti dal file associato
    for(i=0;i<M;i++)
	{   

   		if ((pidFiglio = wait(&status)) < 0)
   		{
      		printf("Errore wait\n");
      		exit(8);
   		}
   		if ((status & 0xFF) != 0)
            printf("ERR:Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
   		else
   		{
            //In caso di successo devo printare il valore di ritorno del figlio, che metto all' interno di ritorno
            ritorno=(int)((status >> 8) & 0xFF);
            printf("Il figlio con pid=%d ha scritto esattamente %d caratteri letti dal file %s (se 255 errore) \n", pidFiglio, ritorno,argv[m]);

        }
	}
    exit(0);
}