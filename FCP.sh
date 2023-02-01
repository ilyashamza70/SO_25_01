#!/bin/bash
#   FIle principale FCP.sh della prova del 25/01/2023


#Per prima cosa proseguo con la verifica del numero di parametri con cui è stato chiamato

case $# in
    0|1|2)
        echo ERROR: numero di parametri = $# è inferiore a tre, attesa chiamta cosi: $0 dirass1 stringa1 string2
        exit 1;;
    3)  #Se sono qua controllo su param OK e proseguo
        #echo DEBUG: num param $# corretto
        ;;
    *)
        echo ERROR: numero di parametri = $# è superiore a tre
        exit 2;;
esac

#Verificato il numero di param proseguo al controllo dei singoli parametri
#Il primo è una DIR ASSOLUTA
case $1 in 
    /*) #Se sono qua vuol dire che è assoluto, ora verifico che sia una directory
        if test ! -d $1 -o ! -x $1
        then
            echo ERROR: il primo param $1 NON è una Directory o NON è atrraversabile
            exit 3
        fi;; 
    *)
        echo ERROR: Il primo parametro $1 NON è in form assoluta
        exit 4;;
esac
#Salvo il primo param in g ed eseguo shift
g=$1
shift

#Ora verifico che il secondo e terzo param siano stringhe
#Per verificare che sia una stringa verifco con case 

case $1 in
    [!a-zA-Z]*)
        echo ERROR il secondo param non è una stringa !
        exit 5;;
    *)
        ;;
esac
#Salvo Secondo param in S1 ed eseguo shift
S1=$1
shift;

#Faccio lo stesso per il terzo parametro
case $1 in
    [!a-zA-Z]*)
        echo ERROR il terzo param non è una stringa !
        exit 5;;
    *)
        ;;
esac

#Salvo Terzo param in S1 ed eseguo shift
S2=$1
shift

#Verifico in debugging se ha un comportamento corretto
#echo DEBUG: verificati param e salvati $g $S1 $S2


#Importante salvare il path
PATH=`pwd`:$PATH
#Devo anche eseguire export cosi da poterla usare anche nel File ricorsivo FCR.sh
export PATH

# AVVIO delle Fase SINGOLA di Ricorsive di esplorazioni in g

FCR.sh $g $S1 $S2

#echo DEBUG: FINITA RICORSIONE



#echo FIne bash Life


