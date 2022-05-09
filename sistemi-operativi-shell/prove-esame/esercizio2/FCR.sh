#!/bin/sh
#file comandi ricorsivo 

cd $1

#NR ci serve per calcolare il numero di linee
NR=

#TROVATO ci serve per sapere se abbiamo trovato almeno un file
TROVATO=false

for i in *
do 
	if test -f $i -a -r $i
	then 
		NR=`wc -l < $i`
		if test $NR -eq $2
		then 
			echo `pwd` /$i >> $3
		TROVATO=true
		fi
	fi
done

if test $TROVATO = true
then 
	echo TROVATO directory `pwd`
fi

for i in *
do 
	if test -i $i -a -x $i
	then 
		$0 `pwd`/$i $2 $3
	fi 
done

