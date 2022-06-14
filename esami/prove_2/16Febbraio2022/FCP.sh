#!/bin/sh
#file comandi principale
#controllo numero parametri

case $# in
	0|1|2)
		echo error: numero parametri errato
		exit 1;;
	*)
		;;
esac

#controllo primo paramtro, carattere minuscolo
case $1 in 
	?)
		case $i in 
			[!a-z])
				echo error: non carattere minuscolo
				exit 2;;
			*)
				;;
	*)
		echo error: non singolo carattere alfabetico minuscolo
		exit 3;;
esac

#salviamo il primo carattere e facciamo lo shift
C=$1
shift

#controllo altri Q parametri, nomi assoluti
for i in $*
do 
	case $i in 
		/*)
			if test ! -d $i -o ! -x $i
			then 
				echo error: non directory
				exit 4
			fi;;
		*)
			echo error: non nome assoluto
			exit 5;;
	esac
done

#fine controllo parametri
#settiamo la variabile PATH
PATH=`pwd`:PATH
export PATH

#creiamo un file tmp
> /tmp/nomiAssoluti

#fasi
for G
do 
	echo fase per $G
	FCR.sh $G $C /tmp/nomiAssoluti
done

NF=`wc -l < /tmp/nomiAssoluti`
echo trovati $NF file
if test $NF -ge 2
then
	#chiamiamo la parte c
	main `cat /tmp/nomiAssoluti` $C
fi

rm /tmp/nomiAssoluti
