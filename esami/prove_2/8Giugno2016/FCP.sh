#!/bin/sh
#file comandi principale

#controllo numero parametri
case $# in 
	4)
		;;
	*)
		echo error: numero parametri errato
		exit 1;;
esac

#controllo primi due parametri, nomi assoluti
case $1 in 
	/*)
		if test ! -d $1 -o ! -x $1
		then 
			echo error: non directory
			exit 2
		fi;;
	*)
		echo error: non nome assoluto
		exit 3;;
esac

case $2 in 
	/*)
		if test ! -d $2 -o ! -x $2
		then
			echo error: non directory
			exit 4
		fi;;
	*)
		echo erro: non nome assoluto
		exit 5;;
esac

#controllo terzo parametro, nome relativo semplice
case $3 in 
	*/*) 
		echo error: non nome relativo semplice
		exit 6;;
	*)
		;;
esac

#controllo quarto parametro, numero poisitivo minore di 255
expr $4 + 0 > /dev/null 2>&1
N4=$?
if test $N4 -ne 2 -o $N4 -ne 3
then 
	if test $4 -le 0 -o -gt 255
	then 
		echo error: numero non compreso tra 1 e 255
		exit 7
	fi
fi

#fine controlli
#settiamo la variabile PATH
PATH=`pwd`:PATH
export PATH

> /tmp/files$$

echo fase per $1
FCR.sh $1 $3 $4 /tmp/files$$

echo fase per $2
FCR.sh $2 $3 $4 /tmp/files$$

NF=`wc -l < /tmp/files$$`
if test $NF -ge 2
then 
	echo invociamo parte c
	main `cat /tmp/files$$` $4
fi

rm /tmp/files$$


