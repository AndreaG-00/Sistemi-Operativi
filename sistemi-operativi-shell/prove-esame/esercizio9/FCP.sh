#!/bin/sh
#file comandi principale 
#esame del 15 Febbraio 2017

#controllo sul numero dei parametri
case $# in 
	2) echo OK;;
	*) echo error: numero parametri errato
		exit 1;;
esac

#controllo primo parametro
case $1 in 
	/*) if test ! -d $1 -o ! -x $1
		then 
			echo error: non directory o non traversabile
			exit 2
		fi;;
	*) echo error: non nome assoluto
		exit 3;;
esac

#controllo secondo parametro
expr $2 + 0 > /dev/null 2>&1
N1=$?
if test $N1 -ne 2 -o $N1 -ne 3
then 
	if test $2 -le 0
	then 
		echo error: numero negativo
		exit 4
	fi
else 
	echo error: non numero
	exit 5
fi

#fine controlli
#settiamo la variabile PATH
PATH=`pwd`:PATH
export PATH

#chiamiamo il file comandi ricorsivo
FCR.sh $*

#fine file comandi principale

