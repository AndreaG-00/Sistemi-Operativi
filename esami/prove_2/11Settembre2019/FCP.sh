#!/bin/sh
#file comandi principale

#controllo numero parametri
case $# in 
	0|1|2|3)
		echo error: numero parametri errato
		exit 1;;
	*)
		;;
esac

#controllo primo parametro, numero positivo
exec $1 + 0 > /dev/null 2>&1
N1=$?
if test $N1 -ne 2 -o $N1 -ne 3
then 
	if test $1 -le 0
	then 
		echo error: numero non positivo
		exit 2
	fi
else
	echo error: non numero
	exit 3
fi
H=$1

#controllo secondo parametro, numero compreso tra 1 e 255
expr $2 + 0 > /dev/null 2>&1
N2=$?
if test $N2 -ne 2 -o $N2 -ne 3
then 
	if test $2 -le 1 -o $2 -gt 255
	then 
		echo error: numero non compreso tra 1 e 255
		exit 4
	fi
else
	echo error: non numero
	exit 5
fi
K=$2

#facciamo lo shift per entrambi i parametri
shift
shift

#controllo ultime W gerarchie
for W in $*
do 
	case $W in 
		/*)
			if test ! -d $W -o ! -x $W
			then 
				echo error: non directory
				exit 6
			fi;;
		*)
			echo error: non nome assoluto
			exit 7;;
	esac
done

#fine controllo parametri
#settiamo la variabile PATH
PATH=`pwd`:PATH
export PATH

#fasi
for i
do
	echo fase per $i
	FCR.sh $i $H $K
done
