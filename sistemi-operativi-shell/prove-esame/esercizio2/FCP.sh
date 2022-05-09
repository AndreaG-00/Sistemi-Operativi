#!/bin/sh
#file comandi principale 

#controllo numero dei parametri
case $# in 
	2) ;;
	*) echo ERROR: numero parametri sbagliato
		exit 1;;
esac

#controllo primo parametro (nome assoluto di una directory)
case $1 in
	/*)if test ! -i $1 ! -x $1
		then 
			echo "non directory o non traversabile"
		exit 2
		fi ;;

	*) echo ERROR: $1 non nome assoluto
	exit 2 ;;
esac

#controllo secondo parametro
expr $2 + 0 > /dev/null 2>&1
N1=$?
if test $N1 -ne 2 -a $N1 -ne 3
then 
	echo $2 numero
	if test $2 -le 0
	then 
	echo $2 numero negativo
	exit 3
	fi
else 
	echo $2 non numero 
	exit 4
fi

#settaggio variabile PATH
PATH=`pwd`:$PATH
export PATH

#invochiamo file comandi ricorsivo
FCR.sh $*

#fine file comandi principale





