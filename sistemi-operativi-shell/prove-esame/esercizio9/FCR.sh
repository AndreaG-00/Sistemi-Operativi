#!/bin/sh
#file comandi ricorsivo

#parametri
#primo parametro -> nome directory
#secondo parametro -> numero intero positivo

#il file deve cercare nella gerarchia le directory che contengono almeno un 
#file leggibile con lunghezza pari a X linee

#poi il contenuto del file deve avere per ogni linea almeno 
#un carattere numerico

cd $1

NR= #numero righe

NRC= #numero righe che contengono almeno un carattere numerico

files= #variabile per memorizzare i file

for i in *
do 
	if test -f $i -a -r $i 
	then 
		NR=`wc -l < $i`
		NRC=`grep '[0-9]' $i | wc -l`

		if test $NR -eq $2 -a $NRC -eq $NR
		then 
			files="$files $i"
		fi
	fi 		
done

if test "$files"
then 
	echo trovata directory `pwd`
	echo che contiene i seguenti file $files
	echo CHIAMO LA PARTE C\?
	read risposta 
	case $risposta in 
		s* | S* | y* | Y*) echo CHIAMO LA PARTE C;;
		*) echo nessuna invocazione;;
		esac
fi

for i in * 
do 
	 if test -d $i -a -x $i
	 then 
		$0 `pwd`/$i $2
	 fi
done

#fine file comandi ricorsivo

