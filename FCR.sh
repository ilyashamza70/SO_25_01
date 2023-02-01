#!/bin/bash
#  FIle Ricorsivo FCR.sh della prova del 25/01/2023
#Per prima cosa mi sposto nella current directory g 
cd $1
count1=0    #variabili usate per contare i file salvati
count2=0    #Stesso vale per count_2

curr=   #inizializza list con nomi file da usare in chiamata con main
>/tmp/nomiRicerca-$$    #file tmp con tutti i file su cui eseguire chiamata a main

#Uso anche S1 e S2 in cui salvo le relative terminazioni di ricerca
S1=$2
S2=$3

#echo DEBUG: SONO IN `pwd` e cerco terminazione con $S1 e $S2

#creo due file tmp che elimino alla fine di ogni ciclo con i nomi dei file

for f in *
do
    #Verifco che il file sia file e readable. 
    if test -f $f -a -r $f 
    then
        #Se sono qua vuol dire che ho trovato una file  
        #Ora verifico il suo nome

        #Verifico prima se il file termina con S1 e quindi solo readable
        #echo DEBUG: TROVATO FILE CON NOME $f ora verifico terminazione
        # DEVO USARE UNA CASE PER VERIFICARE il nome del file
        case $f in 
            *.$S1)
                
                #echo ++++++TROVATO FILE $f CHE TERMINA CON $S1++++++
                count1=`expr $count1 + 1`
                curr="$curr $f"
                ;;
            *.$S2)  #Poi verifico se termina con s2
                    #Se termina con S2 se non ricordo male deve essere scrivibile solo questo
                if test -w $f
                then
                    #echo ++++++TROVATO FILE $f CHE TERMINA CON $S2 ed e leggibile+++++
                    count2=`expr $count2 + 1`
                    echo $f >> /tmp/nomiRicerca-$$
                fi
                ;;
        esac
    fi
done

#Printo nome direttorio se contiene almeno 2 file una per terminazione
#PEr ogni file con terminazione s2 inoltre chiamo il main con tutti i file con tesrminazione s1
sum=`expr $count1 + $count2`
if test $sum -ge 2
then 
    echo Trovato directory che soddisfa specifiche di nome `pwd`
    echo Esattamente ha $count1 file che terminano con $S1 e $count2 file terminanti con $S2
    for i in `cat /tmp/nomiRicerca-$$`
    do  
        #echo ------------CHIAMATA A MAIN di $i-----------------
        #echo pareametri passati main $curr $i
        main $curr $i
    done 
fi

rm /tmp/nomiRicerca-$$

#Eseguo un altro ciclo attraverso * per cercare le Sottodirectory
#In caso Affermativo faccio una chiamata Ricorsiva al file ricorsivo stess
#Dove pero cambio il primo param con la sottodirectory trovata

for D in *
do
    if test -d $D -a -x $D
    then
        $0 `pwd`/$D $2 $3 $4
    fi
done
